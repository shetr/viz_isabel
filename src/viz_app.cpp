#include "viz_app.h"

#include <numeric>

#include <intrin.h>

struct VertexPosUV
{
    glm::vec3 pos;
    glm::vec2 uv;
};

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

void VizApp::Init(Window* window, VizImGuiContext* imguiContext)
{
    _window = window;
    _imguiContext = imguiContext;

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

    GL(Disable(GL_CULL_FACE));
    GL(Enable(GL_DEPTH_TEST));
    GL(ClearColor(0, 0, 0, 1));
}

void VizApp::Update(double deltaTime)
{
    glm::ivec2 winSize = _window->GetSize();
    GL(Viewport(0, 0, winSize.x, winSize.y));
    float aspectRatio = (float) winSize.x / (float) winSize.y;
    glm::vec2 cursorPos = _window->GetCursorPosEye();
    glm::vec2 cursorDir = cursorPos - _lastCursorPos;

    if (!ImGui::IsAnyWindowHovered() && !ImGui::IsAnyWindowFocused() && _window->IsMouseButtonPressed(MouseButton::LEFT_1)) {
        _camera.GetRotY() -= _camera.GetSpeed() * cursorDir.x;
        _camera.GetRotX() -= _camera.GetSpeed() * cursorDir.y;
    }

    _camera.Recalculate(aspectRatio);
    glm::vec3 cutPos = glm::vec3(0, 0, _cutZ - 0.5f);
    glm::mat4 M = glm::translate(cutPos);
    glm::mat4 PVM = _camera.GetP() * _camera.GetV() * M;
    
    GL(Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    _cutShader->SetUniformMat4("u_PVM", PVM);
    _cutShader->SetUniformInt("u_textureSampler", 0);
    _cutShader->SetUniformFloat("u_minTemp", g_minTemp);
    _cutShader->SetUniformFloat("u_maxTemp", g_maxTemp);
    _cutShader->SetUniformFloat("u_z", _cutZ);

    _cutShader->Bind();
    _tempTexture->BindToUnit(0);
    _cutVertexArray->Bind();

    GL(DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    
    _cutShader->Unbind();
    _cutVertexArray->UnBind();

    {
        ImGuiDraw imguiDraw = _imguiContext->Draw();

        ImGui::Begin("Test");

        ImGui::SliderFloat("cut pos", &_cutZ, 0, 1);
        
        ImGui::SliderFloat("rot y", &_camera.GetRotY(), -180, 180);
        ImGui::SliderFloat("rot x", &_camera.GetRotX(), -89, 89);
        ImGui::SliderFloat("cam speed", &_camera.GetSpeed(), 0, 500);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", deltaTime * 1000.0, 1.0 / deltaTime);

        ImGui::End();
    }

    _lastCursorPos = cursorPos;
}