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
    while (!_window->ShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
            glVertex3f(-0.7, -0.7, 0.0);
            glVertex3f(0.0, 0.7, 0.0);
            glVertex3f(0.7, -0.7, 0.0);
        glEnd();
        glFlush();

        _window->SwapBuffers();

        glfwPollEvents();
    }
}
