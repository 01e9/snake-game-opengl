#pragma once

#include <gsl/pointers>
#include <set>
#include "./common.hpp"

class GLFWwindow;

namespace app {

struct IObject;

struct IScene {
    virtual IScene& add(gsl::not_null<IObject*> object) = 0;
    virtual IScene& remove(gsl::not_null<IObject*> object) = 0;
    virtual void render() = 0;
    virtual void tick(const std::set<int>& pressedKeys) = 0;

    INTERFACE_COMMON(IScene)
};

}
