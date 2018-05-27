#pragma once

#include <interface/IScene.h>
#include <set>
#include <glm/glm.hpp>
#include <chrono>

namespace app::scene {

class Main : public IScene {
public:
    Main();
    ~Main() = default;

    void add(gsl::not_null<IObject *> object) override;
    void remove(gsl::not_null<IObject *> object) override;
    void render() override;
    void tick(const std::set<int>& pressedKeys) override;

private:
    std::set<IObject*> mObjects;
    glm::mat4 mCamera;
    glm::mat4 mProjection;
    std::chrono::system_clock::time_point mLastCameraMove{std::chrono::system_clock::now()};
};

}
