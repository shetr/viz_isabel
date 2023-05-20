#include "app.h"

App::App(AppImpl* appImpl)
{
    if (!glfwInit()) {
        std::cout << "Failed to init GLFW" << std::endl;
        exit(-1);
    }
    _window = std::unique_ptr<Window>(new Window(640, 480, "VIZ"));
    _imguiContext = std::unique_ptr<VizImGuiContext>(new VizImGuiContext(*_window));
    _appImpl = std::unique_ptr<AppImpl>(appImpl);
}

App::~App()
{
    glfwTerminate();
}

void App::Run()
{
    _appImpl->Init(_window.get(), _imguiContext.get());
    while (!_window->ShouldClose())
    {
        // TODO: add deltaTime
        _appImpl->Update(0);

        _window->SwapBuffers();

        glfwPollEvents();
    }
}
