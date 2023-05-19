#include "common.h"

#include <iostream>

std::vector<std::string> OpenGL::s_openGLErrors = {
    "NO_ERROR",
    "INVALID_ENUM",
    "INVALID_VALUE",
    "INVALID_OPERATION",
    "STACK_OVERFLOW",
    "STACK_UNDERFLOW",
    "OUT_OF_MEMORY",
    "INVALID_FRAMEBUFFER_OPERATION",
    "CONTEXT_LOST"
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