#include "app.h"

App::App()
{
    if (!glfwInit()) {
        std::cout << "Failed to init GLFW" << std::endl;
        exit(-1);
    }
    _window = std::unique_ptr<Window>(new Window(640, 480, "VIZ"));
}

App::~App()
{
    glfwTerminate();
}

void App::Run()
{
    Init();
    while (!_window->ShouldClose())
    {
        Update();

        _window->SwapBuffers();

        glfwPollEvents();
    }
}
