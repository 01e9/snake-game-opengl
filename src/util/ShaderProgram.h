#pragma once

#include <glad/glad.h>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace app::util {

class ShaderProgram {
public:
    explicit ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
    ~ShaderProgram() noexcept;

    inline unsigned int id() const {
        return mId;
    };

    inline void setUniform(const char* name, const glm::mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(mId, name), 1, GL_FALSE, glm::value_ptr(value));
    }

private:
    unsigned int mId;
};

}
