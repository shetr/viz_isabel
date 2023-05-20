#include "viz_app.h"

void VizApp::Init(Window* window, VizImGuiContext* imguiContext)
{
    _window = window;
    _imguiContext = imguiContext;

    GL(ClearColor(0, 0, 0, 1));
}

void VizApp::Update(double deltaTime)
{
    glm::ivec2 winSize = _window->GetSize();
    GL(Viewport(0, 0, winSize.x, winSize.y));
    
    GL(Clear(GL_COLOR_BUFFER_BIT));
}