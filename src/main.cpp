#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gsl/gsl>
#include <stdexcept>
#include <interface/IObject.h>
#include <scene/Main.h>
#include <object/Treat.h>
#include <object/Snake.h>
#include <object/Board.h>
#include <array>
#include <thread>
#include <boost/thread/synchronized_value.hpp>

struct SharedData {
    gsl::not_null<GLFWwindow*> window;
    app::scene::Main scene {};
    std::set<int> pressedKeys {};

    explicit SharedData(gsl::not_null<GLFWwindow*> w): window{w} {}
    SharedData(SharedData &&other) noexcept = default;
    SharedData & operator=(SharedData &&other) noexcept = default;
};

int main()
{
    gsl::not_null window {[] {
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

    auto [ sharedData, _objects /* to keep pointers alive */ ] = [window]{
        auto board{std::make_unique<app::object::Board>()};
        auto treat{std::make_unique<app::object::Treat>(board.get())};
        auto snake{std::make_unique<app::object::Snake>(board.get(), treat.get())};

        auto sharedData{std::make_unique<SharedData>(window)};
        sharedData->scene.add(board.get()).add(treat.get()).add(snake.get());

        return std::make_tuple(
            boost::synchronized_value{std::move(sharedData)},
            std::array<std::unique_ptr<app::IObject>, 3>{ std::move(board), std::move(treat), std::move(snake) }
        );
    }();

    using SharedDataT = decltype(sharedData);
    glfwSetWindowUserPointer(window, &sharedData);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int, int action, int) {
        gsl::not_null sharedData {reinterpret_cast<SharedDataT*>(glfwGetWindowUserPointer(window))};

        do {
            auto d {sharedData->unique_synchronize()};

            if (GLFW_PRESS == action) {
                (*d)->pressedKeys.insert(key);
            } else if (GLFW_RELEASE == action) {
                (*d)->pressedKeys.erase(key);
            } else {
                break;
            }

            (*d)->scene.tick((*d)->pressedKeys);
        } while (false);

        if (GLFW_KEY_ESCAPE == key && action == GLFW_PRESS) {
            glfwSetWindowShouldClose((*sharedData->unique_synchronize())->window, true);
        }
    });

    glfwMakeContextCurrent(nullptr);
    std::jthread renderingThread {[&sharedData](std::stop_token stop_token){
        glfwMakeContextCurrent((*sharedData.unique_synchronize())->window);

        constexpr std::chrono::milliseconds frameInterval {std::milli::den/30};

        while (!stop_token.stop_requested()) {
            auto beginTime {std::chrono::system_clock::now()};

            {
                auto d {sharedData.unique_synchronize()};

                (*d)->scene.render();
                glfwSwapBuffers((*d)->window);
            }

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
    }};

    std::jthread tickThread {[&sharedData](std::stop_token stop_token){
        constexpr std::chrono::milliseconds tickInterval {std::milli::den/100};

        while (!stop_token.stop_requested()) {
            auto beginTime {std::chrono::system_clock::now()};

            {
                auto d {sharedData.unique_synchronize()};

                (*d)->scene.tick((*d)->pressedKeys);
            }

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

    while (!glfwWindowShouldClose(window)) {
        glfwWaitEvents();
    }

    return 0;
}
