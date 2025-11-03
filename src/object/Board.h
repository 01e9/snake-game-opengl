#pragma once

#include <interface/IObject.h>
#include <utility>
#include <vector>
#include <optional>
#include <util/ShaderProgram.h>

namespace app::object {

class Board : public IObject {
public:
    Board();
    ~Board() noexcept;

    IObject& setCamera(const glm::mat4 &view) override;
    IObject& setProjection(const glm::mat4 &projection) override;
    void render() override;

    inline size_t size() const {
        return mSize;
    };
    inline size_t cellSize() const {
        return mCellSize;
    }

private:
    util::ShaderProgram createShaderProgram();
    std::pair<std::vector<float>, std::vector<unsigned int>> generateVeboData() const;
    void createVao();

private:
    size_t mSize;
    size_t mCellSize;
    util::ShaderProgram mShaderProgram;
    unsigned int mVao;
    unsigned int mVbo;
    unsigned int mEbo;
    std::optional<glm::mat4> mPendingCameraUpdate;
    std::optional<glm::mat4> mPendingProjectionUpdate;
};

}
