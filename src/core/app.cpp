#include "app.h"

#include <chrono>

App::App(AppImpl* appImpl)
{
    if (!glfwInit()) {
        std::cout << "Failed to init GLFW" << std::endl;
        exit(-1);
    }
    _window = std::unique_ptr<Window>(new Window(1200, 800, "VIZ"));
    _imguiContext = std::unique_ptr<VizImGuiContext>(new VizImGuiContext(*_window));
    glfwSwapInterval(1);
    _appImpl = std::unique_ptr<AppImpl>(appImpl);
}

App::~App()
{
    glfwTerminate();
}

void App::Run()
{
    _appImpl->Init(_window.get(), _imguiContext.get());
    double deltaTime = 0;
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    while (!_window->ShouldClose())
    {
        _appImpl->Update(deltaTime);

        _window->SwapBuffers();

        glfwPollEvents();
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(now - start).count();
        start = now;
    }
    _appImpl->Shutdown();
}
