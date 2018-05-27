#pragma once

#include <vector>
#include <utility>

namespace app::util {

class Cube {
public:
    static std::pair<std::vector<float>, std::vector<unsigned int>> indexed();
};

}
