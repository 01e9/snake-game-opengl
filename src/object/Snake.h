#pragma once

#include <interface/IObject.h>
#include <gsl/pointers>
#include <array>
#include <deque>
#include <chrono>
#include <util/ShaderProgram.h>
#include <glm/vec2.hpp>
#include <vector>
#include <optional>

namespace app::object {

class Board;
class Treat;

class Snake : public IObject {
public:
    explicit Snake(gsl::not_null<Board*> board, gsl::not_null<Treat*> treat);
    ~Snake();

    void setCamera(const glm::mat4 &view) override;
    void setProjection(const glm::mat4 &projection) override;
    void render() override;

    void tick(const std::set<int> &pressedKeys) override;

private:
    enum class Direction {Up, Down, Left, Right};

private:
    util::ShaderProgram createShaderProgram();
    void createVao();
    void updateNextDirection(const std::set<int> &pressedKeys);
    void move();
    glm::uvec2 getNextHead() const;
    void renderSnake();

    static int getMoveX(Direction direction);
    static int getMoveY(Direction direction);

private:
    Board* mBoard;
    Treat* mTreat;
    util::ShaderProgram mShaderProgram;

    std::deque<std::pair<glm::uvec2, Direction>> mSnake;

    Direction mDirection{Direction::Up};
    Direction mNextDirection{Direction::Up};

    unsigned int mVao;
    unsigned int mVbo;
    unsigned int mEbo;
    unsigned int mInstanceVBO;
    unsigned int mColorVBO;

    unsigned int mIndicesCount;

    static std::chrono::milliseconds mMoveInterval;
    std::chrono::system_clock::time_point mLastMoveTime {std::chrono::system_clock::now()};

    std::optional<glm::mat4> mPendingCameraUpdate;
    std::optional<glm::mat4> mPendingProjectionUpdate;

    bool mSkipTailMove {false};
};

}
