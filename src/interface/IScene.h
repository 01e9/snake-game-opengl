#pragma once

#include <gsl/pointers>
#include <set>

class GLFWwindow;

namespace app {

struct IObject;

struct IScene {
    virtual ~IScene() noexcept = default;

    virtual void add(gsl::not_null<IObject*> object) = 0;
    virtual void remove(gsl::not_null<IObject*> object) = 0;
    virtual void render() = 0;
    virtual void tick(const std::set<int>& pressedKeys) = 0;
};

}
