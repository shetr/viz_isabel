#pragma once

#include "common.h"
#include "window.h"

class App
{
private:
    std::unique_ptr<Window> _window;
public:
    App();
    ~App();

    void Run();

    Window& GetWindow() { return *_window.get(); }
protected:
    virtual void Update() = 0;
};