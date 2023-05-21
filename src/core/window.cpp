#include "window.h"

Window::Window(int width, int height, const std::string& title)
{
    _id = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    glfwMakeContextCurrent(_id);

    if(!gladLoadGL()) {
        std::cout << "Failed to load OpenGL" << std::endl;
        exit(-1);
    }
}

Window::~Window()
{

}

glm::ivec2 Window::GetSize() const
{
    glm::ivec2 size;
    glfwGetWindowSize(_id, &size.x, &size.y);
    return size;
}
void Window::SetSize(int width, int height)
{
    glfwSetWindowSize(_id, width, height);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(_id);
}
bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(_id);
}


glm::vec2 Window::GetCursorPosScreen() const
{
    glm::dvec2 pos;
    glfwGetCursorPos(_id, &pos.x, &pos.y);
    return pos;
}
glm::vec2 Window::GetCursorPosEye() const
{
    return ScreenPosToEyePos(GetCursorPosScreen());
}
glm::vec2 Window::ScreenPosToEyePos(glm::vec2 screenPos) const
{
    glm::vec2 windowSize = GetSize();
    return glm::vec2(2, -2) * screenPos / windowSize + glm::vec2(-1, 1);
}
bool Window::IsMouseButtonPressed(MouseButton button) const
{
    return static_cast<bool>(glfwGetMouseButton(_id, static_cast<int>(button)));
}