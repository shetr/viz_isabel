#pragma once

#include <core/app.h>

#include <graphics/shader.h>
#include <graphics/vertex_array.h>
#include <graphics/texture.h>

class TestApp : public AppImpl
{
private:
    Window* _window;
    VizImGuiContext* _imguiContext;
    std::unique_ptr<Shader> _basicShader;
    std::unique_ptr<VertexBuffer> _triangleVertexBuffer;
    std::unique_ptr<IndexBuffer> _triangleIndexBuffer;
    std::unique_ptr<VertexArray> _triangleVertexArray;
    std::unique_ptr<Texture> _texture;
public:
    TestApp() {}
    
protected:
    void Init(Window* window, VizImGuiContext* imguiContext) override;
    void Update(double deltaTime) override;
};