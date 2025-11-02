#include "Board.h"
#include <glad/glad.h>
#include <stdexcept>
#include <gsl/util>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

namespace app::object {

//region Constructor & Destructor

Board::Board()
    : mSize{13}
    , mCellSize{10}
    , mShaderProgram{createShaderProgram()}
{
    createVao();
}

Board::~Board() {
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &mVao);
    glDeleteFramebuffers(1, &mVbo);
    glDeleteFramebuffers(1, &mEbo);
}

//endregion

//region Public Methods

void Board::render() {
    glUseProgram(mShaderProgram.id());

    if (mPendingCameraUpdate.has_value()) {
        mShaderProgram.setUniform("view", mPendingCameraUpdate.value());
        mPendingCameraUpdate.reset();
    }
    if (mPendingProjectionUpdate.has_value()) {
        mShaderProgram.setUniform("projection", mPendingProjectionUpdate.value());
        mPendingProjectionUpdate.reset();
    }

    glBindVertexArray(mVao);
    glDrawElements(GL_TRIANGLES, std::pow(size(), 2) * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Board::setCamera(const glm::mat4 &view) {
    mPendingCameraUpdate = view;
}

void Board::setProjection(const glm::mat4 &projection) {
    mPendingProjectionUpdate = projection;
}

//endregion

//region Private Methods

util::ShaderProgram Board::createShaderProgram() {
    const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec2 pos;

uniform mat4 view;
uniform mat4 projection;

void main() {
   gl_Position = projection * view * vec4(pos, 0.0, 1.0);
}
)";

    const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.280, 0.276, 0.276, 1.0f);
}
)";

    return util::ShaderProgram{vertexShaderSource, fragmentShaderSource};
}

void Board::createVao() {
    const auto [ vertices, indices ] = generateVeboData();

    unsigned int vbo, ebo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mVao = vao;
    mVbo = vbo;
    mEbo = ebo;
}

std::pair<std::vector<float>, std::vector<unsigned int>> Board::generateVeboData() const {
    constexpr unsigned int verticesPerCell {4};
    constexpr unsigned int vertexSize {2};
    constexpr unsigned int indicesPerCell {2};
    constexpr unsigned int indexSize {3};

    const size_t size {this->size()};

    std::vector<float> vertices {};
    vertices.reserve(size * size * verticesPerCell * vertexSize);

    std::vector<unsigned int> indices {};
    indices.reserve(size * size * indicesPerCell * indexSize);

    const float shift {size / 2.0f};

    for (unsigned int x = 0; x != size; ++x) {
        for (unsigned int y = 0; y != size; ++y) {
            vertices.push_back(x - shift);
            vertices.push_back(shift - y);

            vertices.push_back(x - shift + 1);
            vertices.push_back(shift - y);

            vertices.push_back(x - shift);
            vertices.push_back(shift - y - 1);

            vertices.push_back(x - shift + 1);
            vertices.push_back(shift - y - 1);

            const unsigned int verticesOffset {gsl::narrow_cast<unsigned int>(vertices.size() / vertexSize)};

            indices.push_back(verticesOffset - verticesPerCell);
            indices.push_back(verticesOffset - verticesPerCell + 1);
            indices.push_back(verticesOffset - verticesPerCell + 2);

            indices.push_back(verticesOffset - verticesPerCell + 1);
            indices.push_back(verticesOffset - verticesPerCell + 2);
            indices.push_back(verticesOffset - verticesPerCell + 3);
        }
    }

    // normalize
    std::for_each(vertices.begin(), vertices.end(), [size](float& value){ value /= size; });

    return {vertices, indices};
}

//endregion

}
