#pragma once

#include <glm/glm.hpp>
#include <set>

class GLFWwindow;

namespace app {

struct IObject {
    virtual void setCamera(const glm::mat4& view) = 0;
    virtual void setProjection(const glm::mat4& projection) = 0;
    virtual void render() = 0;
    virtual void tick(const std::set<int>& pressedKeys) {};
};

}
