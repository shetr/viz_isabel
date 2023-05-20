#pragma once

#include <core/app.h>

#include <graphics/shader.h>
#include <graphics/vertex_array.h>

class VizApp : public AppImpl
{
private:
    Window* _window;
    VizImGuiContext* _imguiContext;
    std::unique_ptr<Shader> _basicShader;
    std::unique_ptr<VertexBuffer> _triangleVertexBuffer;
    std::unique_ptr<IndexBuffer> _triangleIndexBuffer;
    std::unique_ptr<VertexArray> _triangleVertexArray;
public:
    VizApp() {}
    
protected:
    void Init(Window* window, VizImGuiContext* imguiContext) override;
    void Update(double deltaTime) override;
};