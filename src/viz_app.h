#pragma once

#include <core/app.h>

#include <graphics/shader.h>
#include <graphics/vertex_array.h>

class VizApp : public AppImpl
{
private:
    Window* _window;
    VizImGuiContext* _imguiContext;
public:
    VizApp() {}
    
protected:
    void Init(Window* window, VizImGuiContext* imguiContext) override;
    void Update(double deltaTime) override;
};