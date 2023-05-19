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

void Window::GetSize(int& width, int& height) const
{
    glfwGetWindowSize(_id, &width, &height);
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
