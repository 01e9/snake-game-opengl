#include "Treat.h"
#include <util/Cube.h>
#include <object/Board.h>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace app::object {

//region Constructor & Destructor

Treat::Treat(gsl::not_null<Board*> board)
    : mBoard{board}
    , mShaderProgram{createShaderProgram()}
{
    createVao();
    setPosition(2, 2);
}

Treat::~Treat() {
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mVbo);
    glDeleteBuffers(1, &mEbo);
}

//endregion

//region Public Methods

void Treat::setCamera(const glm::mat4 &view) {
    mPendingCameraUpdate = view;
}

void Treat::setProjection(const glm::mat4 &projection) {
    mPendingProjectionUpdate = projection;
}

void Treat::render() {
    glUseProgram(mShaderProgram.id());

    if (mPendingCameraUpdate.has_value()) {
        mShaderProgram.setUniform("view", mPendingCameraUpdate.value());
        mPendingCameraUpdate.reset();
    }
    if (mPendingProjectionUpdate.has_value()) {
        mShaderProgram.setUniform("projection", mPendingProjectionUpdate.value());
        mPendingProjectionUpdate.reset();
    }
    if (mPendingModelUpdate.has_value()) {
        mShaderProgram.setUniform("model", mPendingModelUpdate.value());
        mPendingModelUpdate.reset();
    }

    glBindVertexArray(mVao);
    glDrawElements(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

const glm::uvec2 &Treat::position() const {
    return mPosition;
}

const glm::uvec2 &Treat::randomizePosition() {
    return setPosition(
        std::rand() % mBoard->size(),
        std::rand() % mBoard->size()
    );
}

//endregion

//region Private Methods

util::ShaderProgram Treat::createShaderProgram() {
    const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 vertexColor;

void main() {
    vertexColor = color;
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
)";

    const char* fragmentShaderSource = R"(
#version 330 core

in vec3 vertexColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0f);
}
)";

    return util::ShaderProgram{vertexShaderSource, fragmentShaderSource};
}

void Treat::createVao() {
    auto [vertices, indices] = util::Cube::indexed();

    const float colors[] = {
        0.262, 0.513, 0.698,
        0.262, 0.513, 0.698,
        0.262, 0.513, 0.698,
        0.262, 0.513, 0.698,

        0.654, 0.8, 0.905,
        0.654, 0.8, 0.905,
        0.654, 0.8, 0.905,
        0.654, 0.8, 0.905,
    };

    // normalize to board cell size
    std::for_each(vertices.begin(), vertices.end(), [size{mBoard->size()}](float& value){ value /= size; });

    unsigned int vbo, ebo, vao, colorVbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &colorVbo);

    glBindVertexArray(vao);

    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    {
        glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), colors, GL_STATIC_DRAW);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mVao = vao;
    mVbo = vbo;
    mEbo = ebo;

    mIndicesCount = indices.size();
}

const glm::uvec2& Treat::setPosition(unsigned int x, unsigned int y) {
    mPosition = {x, y};

    const auto normalize {
        [boardSize{mBoard->size()}, shift{mBoard->size() / 2}](float coord) -> float {
            return (coord - shift) / boardSize;
        }
    };

    mPendingModelUpdate = glm::translate(glm::mat4(1.0f), glm::vec3{
        normalize(mPosition.x), normalize(mPosition.y), 0.0f
    });

    return mPosition;
}

//endregion

}
