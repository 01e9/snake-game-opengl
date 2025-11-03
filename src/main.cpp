#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gsl/gsl>
#include <stdexcept>
#include <scene/Main.h>
#include <object/Treat.h>
#include <object/Snake.h>
#include <object/Board.h>
#include <thread>
#include <mutex>

using KeyCallbackData = std::tuple<std::set<int>&, bool&, std::mutex&, app::scene::Main&>;

int main()
{
    glfwInit();
    const auto autoTerminateGLFW = gsl::finally([](){ glfwTerminate(); });
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    const auto videoMode {glfwGetVideoMode(glfwGetPrimaryMonitor())};
    if (videoMode == NULL) {
        throw std::runtime_error{"Failed to get GLFW video mode"};
    }

    auto window {glfwCreateWindow(
        gsl::narrow_cast<int>(videoMode->width / 1.6),
        gsl::narrow_cast<int>(videoMode->height / 1.6),
        "Snake Game OpenGL", NULL, NULL
    )};
    if (window == NULL) {
        throw std::runtime_error{"Failed to create GLFW window"};
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        throw std::runtime_error{"Failed to initialize GLAD"};
    }

    glEnable(GL_DEPTH_TEST);

    std::srand(std::time(nullptr));

    app::scene::Main mainScene{};

    app::object::Board boardObject{};
    mainScene.add(&boardObject);

    app::object::Treat treatObject{&boardObject};
    mainScene.add(&treatObject);

    app::object::Snake snakeObject{&boardObject, &treatObject};
    mainScene.add(&snakeObject);

    std::mutex sceneUpdateMutex;
    bool stop {false};

    //region Render thread

    glfwMakeContextCurrent(nullptr);
    std::thread renderingThread {[&window, &mainScene, &stop, &sceneUpdateMutex](){
        glfwMakeContextCurrent(window);

        constexpr std::chrono::milliseconds frameInterval {std::milli::den/30};

        while (!stop) {
            auto beginTime {std::chrono::system_clock::now()};

            {
                std::lock_guard lock{sceneUpdateMutex};
                mainScene.render();
            }
            glfwSwapBuffers(window);

            auto sleepDuration {
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    frameInterval - (std::chrono::system_clock::now() - beginTime)
                )
            };
            if (sleepDuration.count() > 0) {
                std::this_thread::sleep_for(sleepDuration);
            }
        }

        glfwSetWindowShouldClose(window, true);
    }};
    auto autoJoinRenderingThread = gsl::finally([&renderingThread](){
        if (renderingThread.joinable()) {
            renderingThread.join();
        }
    });

    //endregion

    std::set<int> pressedKeys {};

    //region Tick thread

    std::thread tickThread {[&mainScene, &stop, &sceneUpdateMutex, &pressedKeys](){
        constexpr std::chrono::milliseconds tickInterval {std::milli::den/100};

        while (!stop) {
            auto beginTime {std::chrono::system_clock::now()};

            {
                std::lock_guard lock{sceneUpdateMutex};
                mainScene.tick(pressedKeys);
            }

            auto sleepDuration {
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    tickInterval - (std::chrono::system_clock::now() - beginTime)
                )
            };
            if (sleepDuration.count() > 0) {
                std::this_thread::sleep_for(sleepDuration);
            }
        }
    }};
    auto autoJoinTickThread = gsl::finally([&tickThread](){
        if (tickThread.joinable()) {
            tickThread.join();
        }
    });

    //endregion

    //region Key callback

    KeyCallbackData keyCalbackData{pressedKeys, stop, sceneUpdateMutex, mainScene};
    glfwSetWindowUserPointer(window, &keyCalbackData);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int, int action, int) {
        gsl::not_null<KeyCallbackData*> data {reinterpret_cast<KeyCallbackData*>(glfwGetWindowUserPointer(window))};

        auto& [ pressedKeys, stop, sceneUpdateMutex, mainScene ] = *data;

        do {
            std::lock_guard lock{sceneUpdateMutex};

            if (GLFW_PRESS == action) {
                pressedKeys.insert(key);
            } else if (GLFW_RELEASE == action) {
                pressedKeys.erase(key);
            } else {
                break;
            }

            mainScene.tick(pressedKeys);
        } while (false);

        if (GLFW_KEY_ESCAPE == key && action == GLFW_PRESS) {
            stop = true;
        }
    });

    //endregion

    while (!glfwWindowShouldClose(window)) {
        glfwWaitEvents();
    }

    stop = true;

    return 0;
}
