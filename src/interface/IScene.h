#pragma once

#include <gsl/pointers>
#include <set>

class GLFWwindow;

namespace app {

struct IObject;

struct IScene {
    virtual ~IScene() noexcept = default;

    virtual IScene& add(gsl::not_null<IObject*> object) = 0;
    virtual IScene& remove(gsl::not_null<IObject*> object) = 0;
    virtual void render() = 0;
    virtual void tick(const std::set<int>& pressedKeys) = 0;
};

}
