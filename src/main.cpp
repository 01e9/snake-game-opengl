#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gsl/gsl>
#include <stdexcept>
#include <interface/IObject.h>
#include <scene/Main.h>
#include <object/Treat.h>
#include <object/Snake.h>
#include <object/Board.h>
#include <atomic>
#include <array>
#include <thread>
#include <boost/thread/synchronized_value.hpp>

int main()
{
    GLFWwindow* window {[] {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        const auto videoMode {glfwGetVideoMode(glfwGetPrimaryMonitor())};
        if (videoMode == nullptr) {
            throw std::runtime_error{"Failed to get GLFW video mode"};
        }

        auto window {glfwCreateWindow(
            gsl::narrow_cast<int>(videoMode->width / 1.6),
            gsl::narrow_cast<int>(videoMode->height / 1.6),
            "Snake Game OpenGL", nullptr, nullptr
        )};
        if (window == nullptr) {
            throw std::runtime_error{"Failed to create GLFW window"};
        }

        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            throw std::runtime_error{"Failed to initialize GLAD"};
        }

        glEnable(GL_DEPTH_TEST);
        std::srand(std::time(nullptr));

        return window;
    }()};
    const auto _cleanupGLFW = gsl::finally(glfwTerminate);

    auto [ scene, _objects /* keep pointers alive */ ] = []{
        app::scene::Main scene{};

        auto board{std::make_unique<app::object::Board>()};
        scene.add(board.get());

        auto treat{std::make_unique<app::object::Treat>(board.get())};
        scene.add(treat.get());

        auto snake{std::make_unique<app::object::Snake>(board.get(), treat.get())};
        scene.add(snake.get());

        return std::make_tuple(
            boost::synchronized_value{std::move(scene)},
            std::array<std::unique_ptr<app::IObject>, 3>{
                std::move(board),
                std::move(treat),
                std::move(snake)
            }
        );
    }();

    std::atomic stop {false};
    std::set<int> pressedKeys {};

    glfwMakeContextCurrent(nullptr);
    std::jthread renderingThread {[&window, &scene, &stop]{
        glfwMakeContextCurrent(window);

        constexpr std::chrono::milliseconds frameInterval {std::milli::den/30};

        while (!stop.load(std::memory_order_relaxed)) {
            auto beginTime {std::chrono::system_clock::now()};

            scene.unique_synchronize()->render();

            glfwSwapBuffers(window);

            if (
                auto sleepDuration {
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        frameInterval - (std::chrono::system_clock::now() - beginTime)
                    )
                };
                sleepDuration.count() > 0
            ) {
                std::this_thread::sleep_for(sleepDuration);
            }
        }

        glfwSetWindowShouldClose(window, true);
    }};

    std::jthread tickThread {[&scene, &stop, &pressedKeys]{
        constexpr std::chrono::milliseconds tickInterval {std::milli::den/100};

        while (!stop.load(std::memory_order_relaxed)) {
            auto beginTime {std::chrono::system_clock::now()};

            scene.unique_synchronize()->tick(pressedKeys);

            if (
                auto sleepDuration {
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        tickInterval - (std::chrono::system_clock::now() - beginTime)
                    )
                };
                sleepDuration.count() > 0
            ) {
                std::this_thread::sleep_for(sleepDuration);
            }
        }
    }};

    //region Key callback

    using KeyCallbackData = std::tuple<decltype(pressedKeys)&, decltype(stop)&, decltype(scene)&>;

    KeyCallbackData keyCalbackData{pressedKeys, stop, scene};
    glfwSetWindowUserPointer(window, &keyCalbackData);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int, int action, int) {
        gsl::not_null data {reinterpret_cast<KeyCallbackData*>(glfwGetWindowUserPointer(window))};

        auto& [ pressedKeys, stop, scene ] = *data;

        do {
            auto sceneLock = scene.unique_synchronize();

            if (GLFW_PRESS == action) {
                pressedKeys.insert(key);
            } else if (GLFW_RELEASE == action) {
                pressedKeys.erase(key);
            } else {
                break;
            }

            sceneLock->tick(pressedKeys);
        } while (false);

        if (GLFW_KEY_ESCAPE == key && action == GLFW_PRESS) {
            stop.store(true, std::memory_order_relaxed);
        }
    });

    //endregion

    while (!glfwWindowShouldClose(window)) {
        glfwWaitEvents();
    }

    stop = true;

    return 0;
}
