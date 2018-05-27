#include "Cube.h"

namespace app::util {

std::pair<std::vector<float>, std::vector<unsigned int>> Cube::indexed() {
    return {
        {
            //region Bottom
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            //endregion

            //region Top
            -0.5f, 0.5f, 1.0f,
            0.5f, 0.5f, 1.0f,
            0.5f, -0.5f, 1.0f,
            -0.5f, -0.5f, 1.0f,
            //endregion
        },
        {
            //region Faces
            0, 1, 1 + 4,
            0, 0 + 4, 1 + 4,

            1, 2, 2 + 4,
            1, 1 + 4, 2 + 4,

            2, 3, 3 + 4,
            2, 2 + 4, 3 + 4,

            3, 0, 0 + 4,
            3, 3 + 4, 0 + 4,
            //endregion

            //region Bases
            0, 1, 2,
            0, 2, 3,

            0 + 4, 1 + 4, 2 + 4,
            0 + 4, 2 + 4, 3 + 4,
            //endregion
        }
    };
}

}
