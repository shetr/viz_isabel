#include "common.h"

#include <iostream>

std::unordered_map<int, std::string> OpenGL::s_openGLErrors = {
    std::pair<int, std::string>(GL_NO_ERROR, "NO_ERROR"),
    std::pair<int, std::string>(GL_INVALID_ENUM, "INVALID_ENUM"),
    std::pair<int, std::string>(GL_INVALID_VALUE, "INVALID_VALUE"),
    std::pair<int, std::string>(GL_INVALID_OPERATION, "INVALID_OPERATION"),
    std::pair<int, std::string>(GL_STACK_OVERFLOW, "STACK_OVERFLOW"),
    std::pair<int, std::string>(GL_STACK_UNDERFLOW, "STACK_UNDERFLOW"),
    std::pair<int, std::string>(GL_OUT_OF_MEMORY, "OUT_OF_MEMORY"),
    std::pair<int, std::string>(GL_INVALID_FRAMEBUFFER_OPERATION, "INVALID_FRAMEBUFFER_OPERATION"),
    std::pair<int, std::string>(GL_CONTEXT_LOST, "CONTEXT_LOST")
};

GLenum OpenGL::GetError()
{
    return glGetError();
}

void OpenGL::ClearError()
{
    while(OpenGL::GetError() != GL_NO_ERROR);
}

bool OpenGL::Error(const char* file, int line)
{
    GLenum err;
    bool res = false;
    if((err = GetError()) != GL_NO_ERROR) {
        res = true;
        std::cout << "OpenGL error " << s_openGLErrors[err] << ", line " << line << ", file " << file;
    }
    return res;
}