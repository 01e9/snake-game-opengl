#include "Snake.h"
#include <object/Board.h>
#include <object/Treat.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <gsl/util>
#include <limits>
#include <util/Cube.h>
#include <algorithm>

namespace app::object {

//region Static Variables

std::chrono::milliseconds Snake::mMoveInterval {300};

//endregion

//region Constructor & Destructor

Snake::Snake(gsl::not_null<Board *> board, gsl::not_null<Treat *> treat)
    : mBoard{board}
    , mTreat{treat}
    , mShaderProgram{createShaderProgram()}
    , mSnake{
        {{board->size() / 2, 2}, Direction::Up},
        {{board->size() / 2, 1}, Direction::Up},
        {{board->size() / 2, 0}, Direction::Up}}
{
    createVao();
}

Snake::~Snake() noexcept {
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mVbo);
    glDeleteBuffers(1, &mEbo);
    glDeleteBuffers(1, &mInstanceVBO);
    glDeleteBuffers(1, &mColorVBO);
}

//endregion

//region Public Methods

void Snake::tick(const std::set<int> &pressedKeys) {
    updateNextDirection(pressedKeys);

    if (
        std::chrono::system_clock::now() - mLastMoveTime
        >=
        mMoveInterval / (pressedKeys.find(GLFW_KEY_LEFT_SHIFT) != pressedKeys.end() ? 3 : 1)
    ) {
        move();
    }
}

void Snake::render() {
    glUseProgram(mShaderProgram.id());

    if (mPendingCameraUpdate.has_value()) {
        mShaderProgram.setUniform("view", mPendingCameraUpdate.value());
        mPendingCameraUpdate.reset();
    }
    if (mPendingProjectionUpdate.has_value()) {
        mShaderProgram.setUniform("projection", mPendingProjectionUpdate.value());
        mPendingProjectionUpdate.reset();
    }

    renderSnake();

    glUseProgram(0);
}

IObject& Snake::setCamera(const glm::mat4 &view) {
    mPendingCameraUpdate = view;

    return *this;
}

IObject& Snake::setProjection(const glm::mat4 &projection) {
    mPendingProjectionUpdate = projection;

    return *this;
}

//endregion

//region Private Methods

util::ShaderProgram Snake::createShaderProgram() {
    const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in mat4 model;

uniform mat4 view;
uniform mat4 projection;

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

void Snake::createVao() {
    auto [vertices, indices] = util::Cube::indexed();

    const float colors[] = {
        0.7f, 0.321f, 0.129f,
        0.7f, 0.321f, 0.129f,
        0.7f, 0.321f, 0.129f,
        0.7f, 0.321f, 0.129f,

        0.9f, 0.701f, 0.231f,
        0.9f, 0.701f, 0.231f,
        0.9f, 0.701f, 0.231f,
        0.9f, 0.701f, 0.231f,
    };

    // normalize to board cell size
    std::for_each(vertices.begin(), vertices.end(), [size{mBoard->size()}](float& value){ value /= size; });

    unsigned int vbo, ebo, vao, instanceVbo, colorVbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &instanceVbo);
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

    {
        glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * std::pow(mBoard->size(), 2), NULL, GL_DYNAMIC_DRAW);

        for (size_t i = 0; i < 4; ++i) {
            const size_t index {i + 2};
            glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) (sizeof(float) * 4 * i));
            glVertexAttribDivisor(index, 1);
            glEnableVertexAttribArray(index);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mVao = vao;
    mVbo = vbo;
    mEbo = ebo;
    mInstanceVBO = instanceVbo;
    mColorVBO = colorVbo;

    mIndicesCount = indices.size();
}

void Snake::updateNextDirection(const std::set<int> &pressedKeys) {
    if (pressedKeys.find(GLFW_KEY_UP) != pressedKeys.end() && mDirection != Direction::Down) {
        mNextDirection = Direction::Up;
    } else if (pressedKeys.find(GLFW_KEY_DOWN) != pressedKeys.end() && mDirection != Direction::Up) {
        mNextDirection = Direction::Down;
    } else if (pressedKeys.find(GLFW_KEY_LEFT) != pressedKeys.end() && mDirection != Direction::Right) {
        mNextDirection = Direction::Left;
    } else if (pressedKeys.find(GLFW_KEY_RIGHT) != pressedKeys.end() && mDirection != Direction::Left) {
        mNextDirection = Direction::Right;
    }
}

void Snake::move() {
    mLastMoveTime = std::chrono::system_clock::now();
    mDirection = mNextDirection;

    const auto nextHead {getNextHead()};

    if (const auto& treatPos {mTreat->position()}; nextHead.x == treatPos.x && nextHead.y == treatPos.y) {
        mSkipTailMove = true;
        mSnake.pop_back();
        mSnake.push_front({nextHead, mDirection});

        if (mSnake.size() == std::pow(mBoard->size(), 2)) {
            throw std::runtime_error{"Win!"};
        }

        do {
            const auto& treatPos = mTreat->randomizePosition();
        } while (
            std::any_of(mSnake.begin(), mSnake.end(), [&treatPos](const auto& cell){
                return cell.first.x == treatPos.x && cell.first.y == treatPos.y;
            })
        );
    } else {
        if (mSkipTailMove) {
            mSkipTailMove = false;
        } else {
            mSnake.pop_back();
        }

        if (std::any_of(mSnake.begin(), mSnake.end(), [&nextHead](const auto &cell) {
            return cell.first.x == nextHead.x && cell.first.y == nextHead.y;
        })) {
            throw std::runtime_error{"Bump"};
        }

        mSnake.push_front({nextHead, mDirection});
    }
}

glm::uvec2 Snake::getNextHead() const {
    glm::uvec2 nextHead {
        mSnake.front().first.x + getMoveX(mDirection),
        mSnake.front().first.y + getMoveY(mDirection)
    };

    if (nextHead.x == std::numeric_limits<unsigned int>::max()) {
        nextHead.x = mBoard->size() - 1;
    } else if (nextHead.x == mBoard->size()) {
        nextHead.x = 0;
    }

    if (nextHead.y == std::numeric_limits<unsigned int>::max()) {
        nextHead.y = mBoard->size() - 1;
    } else if (nextHead.y == mBoard->size()) {
        nextHead.y = 0;
    }

    return nextHead;
}

int Snake::getMoveX(Direction direction) {
    return (direction == Direction::Right) - (direction == Direction::Left);
}

int Snake::getMoveY(Direction direction) {
    return (direction == Direction::Up) - (direction == Direction::Down);
}

void Snake::renderSnake() {
    std::vector<glm::mat4> snake{};
    snake.reserve(mSnake.size());

    const auto normalize {
        [boardSize{mBoard->size()}, shift{mBoard->size() / 2}](float coord) -> float {
            return (coord - shift) / boardSize;
        }
    };

    const float
        movingScale {1.0f * std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - mLastMoveTime
        ).count() / mMoveInterval.count()},
        movingShift {movingScale / 2};

    auto snakeIt = mSnake.begin();

    snake.push_back(
        glm::scale(
            glm::translate(glm::mat4(1.0f), glm::vec3{
                normalize(
                    snakeIt->first.x
                        + (snakeIt->second == Direction::Right ? movingShift - 0.5f : 0)
                        + (snakeIt->second == Direction::Left ? - movingShift + 0.5f : 0)
                ),
                normalize(
                    snakeIt->first.y
                        + (snakeIt->second == Direction::Up ? movingShift - 0.5f : 0)
                        + (snakeIt->second == Direction::Down ? - movingShift + 0.5f : 0)
                ),
                0.0f
            }),
            glm::vec3{
                (snakeIt->second == Direction::Right || snakeIt->second == Direction::Left) ? movingScale : 1.0f,
                (snakeIt->second == Direction::Up || snakeIt->second == Direction::Down) ? movingScale : 1.0f,
                1.0f
            }
        )
    );

    ++snakeIt;

    for (const auto end = mSnake.end() - 1; snakeIt != end; ++snakeIt) {
        snake.push_back(
            glm::translate(
                glm::mat4(1.0f),
                glm::vec3{normalize(snakeIt->first.x), normalize(snakeIt->first.y), 0.0f}
            )
        );
    }

    const auto lastDirection {(snakeIt - 1)->second};

    if (mSkipTailMove) {
        snake.push_back(
            glm::scale(
                glm::translate(
                    glm::mat4(1.0f),
                    glm::vec3{normalize(snakeIt->first.x), normalize(snakeIt->first.y), 0.0f}
                ),
                glm::vec3{1.0f, 1.0f, 1.0f}
            )
        );
    } else {
        snake.push_back(
            glm::scale(
                glm::translate(
                    glm::mat4(1.0f),
                    glm::vec3{
                        normalize(
                            snakeIt->first.x
                                + (lastDirection == Direction::Right ? movingShift : 0)
                                + (lastDirection == Direction::Left ? -movingShift : 0)
                        ),
                        normalize(
                            snakeIt->first.y
                                + (lastDirection == Direction::Up ? movingShift : 0)
                                + (lastDirection == Direction::Down ? -movingShift : 0)
                        ),
                        0.0f
                    }
                ),
                glm::vec3{
                    (lastDirection == Direction::Right || lastDirection == Direction::Left) ? 1.0f - movingScale : 1.0f,
                    (lastDirection == Direction::Up || lastDirection == Direction::Down) ? 1.0f - movingScale : 1.0f,
                    1.0f
                }
            )
        );
    }

    glBindBuffer(GL_ARRAY_BUFFER, mInstanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * snake.size(), snake.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(mVao);
    glDrawElementsInstanced(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_INT, 0, snake.size());
    glBindVertexArray(0);
}

//endregion

}