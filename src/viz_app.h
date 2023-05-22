#pragma once

#include <core/app.h>
#include <core/arrays.h>

#include <graphics/shader.h>
#include <graphics/vertex_array.h>
#include <graphics/texture.h>
#include <graphics/camera.h>

#include <graphics/primitives.h>

class VizApp : public AppImpl
{
private:
    static VizApp* s_instance;

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
    float _maxVelocityValue = -std::numeric_limits<float>::infinity();
    float _minVelocityValue = std::numeric_limits<float>::infinity();
    
    std::vector<VertexPosVel> _windPts;
    std::vector<int> _windIndices;
    
    std::unique_ptr<Shader> _lineShader;
    std::unique_ptr<VertexBuffer> _lineVertexBuffer;
    std::unique_ptr<IndexBuffer> _lineIndexBuffer;
    std::unique_ptr<VertexArray> _lineVertexArray;

    Camera _camera;
    float _minFOV = 30.0f;
    float _maxFOV = 90.0f;
    glm::vec2 _lastCursorPos;
public:
    VizApp() {}
    
protected:
    void Init(Window* window, VizImGuiContext* imguiContext) override;
    void Update(double deltaTime) override;
    void Shutdown() override;

private:
    static void ErrorCallback(int code, const char* message);
	static void SizeCallback(GLFWwindow* id, int width, int height);
    static void ScrollCallback(GLFWwindow* id, double xoffset, double yoffset);
};