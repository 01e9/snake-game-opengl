#pragma once

#include <interface/IObject.h>
#include <util/ShaderProgram.h>
#include <gsl/pointers>
#include <glm/glm.hpp>

namespace app::object {

class Board;

class Treat : public IObject {
public:
    explicit Treat(gsl::not_null<Board*> board);
    ~Treat();

    void setCamera(const glm::mat4 &view) override;
    void setProjection(const glm::mat4 &projection) override;
    void render() override;
    const glm::uvec2& position() const;
    const glm::uvec2& randomizePosition();

private:
    util::ShaderProgram createShaderProgram();
    void createVao();
    const glm::uvec2& setPosition(unsigned int x, unsigned int y);

private:
    Board* mBoard;
    util::ShaderProgram mShaderProgram;
    glm::uvec2 mPosition;

    unsigned int mVao;
    unsigned int mVbo;
    unsigned int mEbo;

    unsigned int mIndicesCount;

    std::optional<glm::mat4> mPendingCameraUpdate;
    std::optional<glm::mat4> mPendingProjectionUpdate;
    std::optional<glm::mat4> mPendingModelUpdate;
};

}
