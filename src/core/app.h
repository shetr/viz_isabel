#pragma once

#include "common.h"
#include "window.h"
#include <graphics/imgui_context.h>

class AppImpl
{
public:
    virtual void Init(Window* window, VizImGuiContext* imguiContext) = 0;
    virtual void Update(double deltaTime) = 0;
    virtual void Shutdown() = 0;
};

class App
{
private:
    std::unique_ptr<Window> _window;
    std::unique_ptr<VizImGuiContext> _imguiContext;
    std::unique_ptr<AppImpl> _appImpl;
public:
    App(AppImpl* appImpl);
    virtual ~App();

    void Run();

    Window& GetWindow() { return *_window.get(); }
};