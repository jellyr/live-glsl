#pragma once

#include "GLFW/glfw3.h"
#include "log.h"
#include "utils.h"
#include <string>

class Shader {

public:
    Shader();
    ~Shader();

    const GLuint getProgram() const { return program; };
    const GLuint getFragmentShader() const { return fragmentShader; };
    const GLuint getVertexShader() const { return vertexShader; };

    void use() const;
    bool isInUse() const;

    const GLint getAttribLocation(const std::string& attribute);
    bool build(const std::string& fragmentSrc, const std::string& vertexSrc);

    void sendUniform(const std::string& name, float x);
    void sendUniform(const std::string& name, float x, float y);
    void sendUniform(const std::string& name, int size, float* array);

    void detach(GLenum type);

    Log* log;

private:
    GLuint program;
    GLuint fragmentShader;
    GLuint vertexShader;

    GLuint link();
    GLuint compileShader(const std::string& src, GLenum type);
    GLint getUniformLocation(const std::string& uniformName) const;
    void printInfoLog(GLuint shader);
};

static const GLchar* vertexShader = R"END(
attribute vec4 position;

void main() {
    gl_Position = position;
}
)END";

static inline void quad(float* v) {
    short field = 0x0D92; // encode sign
    for(int i = 0; i < 12; ++i) {
        v[i] = (field & 1) == 0 ? -1.0 : 1.0;
        field >>= 1;
    }
}

