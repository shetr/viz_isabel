#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <array>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <memory>
#include <initializer_list>

#define DEBUG_BREAK __debugbreak()

#ifndef _DEBUG
#define GL(action) gl##action
#else 
#define GL(action) gl##action;\
    if(OpenGL::Error(__FILE__, __LINE__)) DEBUG_BREAK;
#endif

class OpenGL
{
private:
    static std::unordered_map<int, std::string> s_openGLErrors;
public:
    static GLenum GetError();
    static void ClearError();
    static bool Error(const char* file, int line);
};
