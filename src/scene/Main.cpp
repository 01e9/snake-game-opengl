#include "Main.h"
#include <GLFW/glfw3.h>
#include <interface/IObject.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace app::scene {

//region Constructor & Destructor

Main::Main()
    : mCamera{glm::lookAt(glm::vec3(0.0f, -0.6f, 1.1f), glm::vec3(0.0f, -0.07f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))}
    , mProjection{glm::perspective(glm::radians(45.0f), 16/9.0f, 0.1f, 100.0f)}
{}

//endregion

void Main::add(gsl::not_null<IObject *> object) {
    object->setCamera(mCamera);
    object->setProjection(mProjection);

    mObjects.insert(object.get());
}

void Main::remove(gsl::not_null<IObject *> object) {
    mObjects.erase(object.get());
}

void Main::tick(const std::set<int> &pressedKeys) {
    // rotate camera
    do {
        const bool left {pressedKeys.find(GLFW_KEY_COMMA) != pressedKeys.end()};
        const bool right {pressedKeys.find(GLFW_KEY_PERIOD) != pressedKeys.end()};

        if (!(left ^ right)) {
            break;
        }

        auto millisecondsAfterLastMove = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - mLastCameraMove
        );
        if (millisecondsAfterLastMove.count() < 30) {
            break;
        }
        mLastCameraMove = std::chrono::system_clock::now();

        mCamera = glm::rotate(mCamera, glm::radians(left ? 1.0f : -1.0f), {0.0f, 0.0f, 1.0f});

        for (const auto object : mObjects) {
            object->setCamera(mCamera);
        }
    } while(false);

    for (const auto object : mObjects) {
        object->tick(pressedKeys);
    }
}

void Main::render() {
    glClearColor(0.180, 0.176, 0.176, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto object : mObjects) {
        object->render();
    }
}

}