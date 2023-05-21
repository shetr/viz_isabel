#pragma once

#include <core/app.h>
#include <core/arrays.h>

#include <graphics/shader.h>
#include <graphics/vertex_array.h>
#include <graphics/texture.h>
#include <graphics/camera.h>

class VizApp : public AppImpl
{
private:
    Window* _window;
    VizImGuiContext* _imguiContext;
    vector3d<float> _isabelTemp; 
    vector3d<glm::vec3> _isabelWind;
    
    std::unique_ptr<Shader> _cutShader;
    std::unique_ptr<VertexBuffer> _cutVertexBuffer;
    std::unique_ptr<IndexBuffer> _cutIndexBuffer;
    std::unique_ptr<VertexArray> _cutVertexArray;
    std::unique_ptr<Texture> _tempTexture;

    std::array<bool, 2> _cutEnabled = {true, true};
    std::array<float, 2> _cuts = {0.5f, 0.5f};
    glm::vec3 _coldColor = glm::vec3(0.0, 74.0, 193.0) / 255.f;
    glm::vec3 _zeroColor = glm::vec3(1.0);
    glm::vec3 _warmColor = glm::vec3(243.0, 43.0, 0.0) / 255.f;
    glm::vec3 _invalidColor = glm::vec3(0.5);

    float _maxTempValue = -std::numeric_limits<float>::infinity();
    float _minTempValue = std::numeric_limits<float>::infinity();
    Camera _camera;
    glm::vec2 _lastCursorPos;
public:
    VizApp() {}
    
protected:
    void Init(Window* window, VizImGuiContext* imguiContext) override;
    void Update(double deltaTime) override;
};