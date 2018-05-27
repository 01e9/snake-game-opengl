#include "ShaderProgram.h"
#include <glad/glad.h>
#include <stdexcept>
#include <gsl/gsl_util>

namespace app::util {

//region Constructor & Destructor

ShaderProgram::ShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource) {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    {
        int success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            throw std::runtime_error{"Vertex shader compilation failed"};
        }
    }
    auto autoDeleteVertexShader = gsl::finally([&vertexShader](){ glDeleteShader(vertexShader); });

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    {
        int success;
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            throw std::runtime_error{"Fragment shader compilation failed"};
        }
    }
    auto autoDeleteFragmentShader = gsl::finally([&fragmentShader](){ glDeleteShader(fragmentShader); });

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    {
        int success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            throw std::runtime_error{"Shader linking failed"};
        }
    }

    mId = shaderProgram;
}

ShaderProgram::~ShaderProgram() {
    glUseProgram(0);
    glDeleteProgram(mId);
}

//endregion

}
