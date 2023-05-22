#include "viz_app.h"

#include <numeric>

#include <intrin.h>

VertexPosUV g_quad[] = {
   {{-0.5, -0.5, 0.0}, {0, 0}},
   {{-0.5,  0.5, 0.0}, {0, 1}},
   {{ 0.5,  0.5, 0.0}, {1, 1}},
   {{ 0.5, -0.5, 0.0}, {1, 0}}
};
int g_quadIndices[] = {0, 1, 2, 0, 2, 3};

const float g_minTemp = -83.00402f;
const float g_maxTemp = 31.51576;

void ReadVolumetricFile(const std::string& filename, vector3d<float>& out)
{
    FILE* f = fopen(filename.c_str(), "r");
    if (!f) {
        std::cout << "Failed to read file: " << filename << std::endl;
        return;
    }
    vector3d<float> temp(glm::uvec3(out.dim().x, out.dim().z, out.dim().y), 0.0f);
    fread(temp.data(), temp.size()*sizeof(float), 1, f);
    fclose(f);
    // big endian to little endian
    for (size_t i = 0; i < temp.size(); ++i) {
        uint32_t v = *(uint32_t*)&temp[i];
        v = _byteswap_ulong(v);
        temp[i] = *(float*)&v;
    }
    // switch y and z axis
    for (size_t z = 0; z < out.dim().z; ++z) {
        for (size_t y = 0; y < out.dim().y; ++y) {
            for (size_t x = 0; x < out.dim().x; ++x) {
                out(x, y, z) = temp(x, z, y);
            }
        }
    }
}

VizApp* VizApp::s_instance = nullptr;

void VizApp::Init(Window* window, VizImGuiContext* imguiContext)
{
    s_instance = this;

    _window = window;
    _imguiContext = imguiContext;
    
    glfwSetErrorCallback(&ErrorCallback);

    std::string cutShaderVS = std::string(SHADERS_SRC_LOC) + "cut.vs";
    std::string cutShaderFS = std::string(SHADERS_SRC_LOC) + "cut.fs";
    _cutShader = std::unique_ptr<Shader>(new Shader(cutShaderVS, cutShaderFS));

    glm::uvec3 data_size(500, 100, 500);
    _isabelTemp = vector3d(data_size, 0.0f);
    _isabelWind = vector3d(data_size, glm::vec3(0));

    std::string tempFilename = std::string(DATA_LOC) + "TCf24.bin";
    ReadVolumetricFile(tempFilename, _isabelTemp);
    char axis[3] = {'U', 'W', 'V'};
    vector3d<float> windAxis(data_size, 0.0f);
    for (int ax = 0; ax < 3; ++ax) {
        std::string windAxisFilename = std::string(DATA_LOC) + axis[ax] + std::string("f24.bin");
        ReadVolumetricFile(windAxisFilename, windAxis);
        for (size_t i = 0; i < windAxis.size(); ++i) {
            _isabelWind[i][ax] = windAxis[i];
        }
    }
    
    VertexLayout cutVerticesLayout = {
        VertexElement(GL_FLOAT, 3),
        VertexElement(GL_FLOAT, 2)
    };
    _cutVertexBuffer = std::unique_ptr<VertexBuffer>(new VertexBuffer(4 * sizeof(VertexPosUV), g_quad, cutVerticesLayout));
    _cutIndexBuffer = std::unique_ptr<IndexBuffer>(new IndexBuffer(6 * sizeof(int), g_quadIndices));
    _cutVertexArray = std::unique_ptr<VertexArray>(new VertexArray(*_cutVertexBuffer, *_cutIndexBuffer));

    _tempTexture = std::unique_ptr<Texture>(new Texture(GL_TEXTURE_3D, GL_R32F, GL_RED, GL_FLOAT));
    _tempTexture->SetData3D(data_size.x, data_size.y, data_size.z, _isabelTemp.data());

    _lastCursorPos = _window->GetCursorPosEye();

    // Wind line glyph initialization
    std::string lineShaderVS = std::string(SHADERS_SRC_LOC) + "line.vs";
    std::string lineShaderFS = std::string(SHADERS_SRC_LOC) + "line.fs";
    _lineShader = std::unique_ptr<Shader>(new Shader(lineShaderVS, lineShaderFS));

    //_windPts.reserve(data_size.x * data_size.y * data_size.z * 3 * 2); // array for uniform lines vbuffer 
    //GenerateLines(_isabelWind, _windPts);

    _windPts.reserve(data_size.x * data_size.z * 2); // array for uniform lines vbuffer
    GenerateLinesLayerHorizontal(_isabelWind, _cuts[1], _windPts, _windIndices, 16);

    for (const VertexPosVel& vertex : _windPts)
    {
        _maxVelocityValue = std::max(vertex.vel, _maxVelocityValue);
        _minVelocityValue = std::min(vertex.vel, _minVelocityValue);
    }

    VertexLayout lineVerticesLayout = {
        VertexElement(GL_FLOAT, 3),
        VertexElement(GL_FLOAT, 1),
        VertexElement(GL_FLOAT, 3)
    };

    _lineVertexBuffer = std::unique_ptr<VertexBuffer>(new VertexBuffer(_windPts.size() * sizeof(VertexPosVel), _windPts.data(), lineVerticesLayout, GL_DYNAMIC_STORAGE_BIT));
    _lineIndexBuffer = std::unique_ptr<IndexBuffer>(new IndexBuffer(_windIndices.size() * sizeof(int), _windIndices.data()));
    _lineVertexArray = std::unique_ptr<VertexArray>(new VertexArray(*_lineVertexBuffer, *_lineIndexBuffer));

    GL(Disable(GL_CULL_FACE));
    GL(Enable(GL_DEPTH_TEST));
    GL(ClearColor(0, 0, 0, 1));
    
    glfwSetWindowSizeCallback(_window->GetID(), &SizeCallback);
    glfwSetScrollCallback(_window->GetID(), &ScrollCallback);
}

void VizApp::Update(double deltaTime)
{
    glm::ivec2 winSize = _window->GetSize();
    GL(Viewport(0, 0, winSize.x, winSize.y));
    GL(Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    float aspectRatio = (float) winSize.x / (float) winSize.y;
    glm::vec2 cursorPos = _window->GetCursorPosEye();
    glm::vec2 cursorDir = cursorPos - _lastCursorPos;
    if (!ImGui::IsAnyWindowHovered() && !ImGui::IsAnyWindowFocused() && _window->IsMouseButtonPressed(MouseButton::LEFT_1)) {
        _camera.GetRotY() -= _camera.GetMovementSpeed() * cursorDir.x;
        _camera.GetRotX() -= _camera.GetMovementSpeed() * cursorDir.y;
    }
    _camera.Recalculate(aspectRatio);

    _windPts.clear();
    _windIndices.clear();
    GenerateLinesLayerHorizontal(_isabelWind, _cuts[1], _windPts, _windIndices, 16);
    _lineVertexBuffer->SetData(0, _windPts.size() * sizeof(VertexPosVel), _windPts.data());

    // draw temp cuts
    for (int cut = 0; cut < (int)_cuts.size(); ++ cut) {
        if (!_cutEnabled[cut]) {
            continue;
        }
        glm::mat4 M;
        if (cut == 0) {
            M = glm::translate(glm::vec3(0, 0, _cuts[cut] - 0.5f));
        } else {
            M = glm::translate(glm::vec3(0, _cuts[cut] - 0.5f, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0));
        }
        glm::mat4 PVM = _camera.GetP() * _camera.GetV() * M;

        _cutShader->SetUniformMat4("u_PVM", PVM);
        _cutShader->SetUniformInt("u_textureSampler", 0);
        _cutShader->SetUniformFloat("u_minTemp", g_minTemp);
        _cutShader->SetUniformFloat("u_maxTemp", g_maxTemp);
        _cutShader->SetUniformFloat("u_shift", _cuts[cut]);
        _cutShader->SetUniformInt("u_axis", cut);
        _cutShader->SetUniformFloat3("u_coldColor", _coldColor);
        _cutShader->SetUniformFloat3("u_zeroColor", _zeroColor);
        _cutShader->SetUniformFloat3("u_warmColor", _warmColor);
        _cutShader->SetUniformFloat3("u_invalidColor", _invalidColor);

        _cutShader->Bind();
        _tempTexture->BindToUnit(0);
        _cutVertexArray->Bind();

        GL(DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
        
        _cutShader->Unbind();
        _cutVertexArray->UnBind();

    }
    
    // Draw wind line glyphs
    {
        glm::mat4 PVM = _camera.GetP() * _camera.GetV();

        _lineShader->SetUniformMat4("u_PVM", PVM);
        _lineShader->SetUniformFloat("u_minVelocity", _minVelocityValue);
        _lineShader->SetUniformFloat("u_maxVelocity", _maxVelocityValue);

        _lineShader->Bind();
        _lineVertexArray->Bind();
        
        //GL(DrawElements(GL_LINES, 500*500*2*3, GL_UNSIGNED_INT, 0));
        GL(DrawElements(GL_TRIANGLES, _windIndices.size(), GL_UNSIGNED_INT, 0));
        
        _lineShader->Unbind();
        _lineVertexArray->UnBind();
    }
    
    
    {
        ImGuiDraw imguiDraw = _imguiContext->Draw();

        ImGui::Begin("Test");
        
        if (ImGui::BeginTabBar("##TabBar"))
        {
            if (ImGui::BeginTabItem("Main")) {
                ImGui::Checkbox("Z Cut", &_cutEnabled[0]);
                if (_cutEnabled[0])
                    ImGui::SliderFloat("Z cut pos", &_cuts[0], 0, 1);
                ImGui::Checkbox("Y Cut", &_cutEnabled[1]);
                if (_cutEnabled[1])
                ImGui::SliderFloat("Y cut pos", &_cuts[1], 0, 1);

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Other")) {
                ImGui::Text("Cut colors");
                ImGui::ColorEdit3("cold color", &_coldColor[0]);
                ImGui::ColorEdit3("zero color", &_zeroColor[0]);
                ImGui::ColorEdit3("warm color", &_warmColor[0]);
                ImGui::ColorEdit3("invalid color", &_invalidColor[0]);
                
                ImGui::SliderFloat("rot y", &_camera.GetRotY(), -180, 180);
                ImGui::SliderFloat("rot x", &_camera.GetRotX(), -89, 89);
                ImGui::SliderFloat("cam speed", &_camera.GetMovementSpeed(), 0, 500);
                ImGui::SliderFloat("cam FOV", &_camera.GetFOV(), _minFOV, _maxFOV);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", deltaTime * 1000.0, 1.0 / deltaTime);
                
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }

        ImGui::End();
    }

    _lastCursorPos = cursorPos;
}

void VizApp::Shutdown()
{
    s_instance = nullptr;
}

void VizApp::ErrorCallback(int code, const char* message)
{
    if (s_instance) {
        std::cout << "GLFW error: " << message << std::endl;
    }
}

void VizApp::SizeCallback(GLFWwindow* id, int width, int height)
{
    //std::cout << "Resized: " << width << ", " << height << std::endl;
}

void VizApp::ScrollCallback(GLFWwindow* id, double xoffset, double yoffset)
{
    //std::cout << "Scroll: " << xoffset << ", " << yoffset << std::endl;
    s_instance->_camera.GetFOV() += (float)yoffset * s_instance->_camera.GetZoomSpeed();
    if (s_instance->_camera.GetFOV() < s_instance->_minFOV) {
        s_instance->_camera.GetFOV() = s_instance->_minFOV;
    }
    if (s_instance->_camera.GetFOV() > s_instance->_maxFOV) {
        s_instance->_camera.GetFOV() = s_instance->_maxFOV;
    }
}