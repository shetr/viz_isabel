#include "viz_app.h"

#include <numeric>
#include <algorithm>
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

const float g_invalidValueLimit = 1.0e34;

glm::vec4 LegendGradient::ComputeInterpolatedColor(float val)
{
    glm::vec4 ret = glm::vec4(1.f);

    std::sort(gradients.begin(), gradients.end(), [](const Gradient& a, const Gradient& b) {
        return a.weight < b.weight;
    });

    if (gradients.size() > 1)
    {
        for (int i = 0; i < gradients.size(); i++)
        {
            if (i + 1 < gradients.size())
            {
                if (gradients[i].weight <= val && gradients[i+1].weight >= val)
                {
                    float t = (val - gradients[i].weight) / (gradients[i + 1].weight - gradients[i].weight);
                    ret = gradients[i].color * glm::vec4(glm::vec3(1.f) - glm::vec3(t), 1.f) + gradients[i + 1].color * glm::vec4(glm::vec3(t), 1.f);
                    return ret;
                }
            }
        }
    }
    else if (gradients.size() == 1)
    {
        return glm::vec4(gradients[0].color.x, gradients[0].color.y, gradients[0].color.z, gradients[0].color.w);
    }

    return ret;
}

Gradient::Gradient(float pos, glm::vec4 col) : weight(pos), color(col)
{

}

/*
void VizApp::GradientGeneratorWindow()
{
    if (ImGui::Begin(("Gradient color editor##")))
    {
        static int focusedGradient = -1;

        if (ImGui::Button("Add Gradient"))
        {
            if (_gradients.size() > 0)
            {
                auto& posVal = _gradients[_gradients.size() - 1].weight;
                posVal *= 0.5f;
                _gradients.push_back(Gradient(posVal, glm::vec4(1,0,1,1)));
            }
            else
            {
                _gradients.push_back(Gradient(1.f, glm::vec4(1,0,1,1)));
            }
            focusedGradient = -1;
        }

        float sum_max = 1.f;
        
        for (size_t i = 0; i < _gradients.size(); i++)
        {
            auto& col = _gradients[i].color;
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(col.x, col.y, col.z, col.w));
            float oldVal = _gradients[i].weight;
            auto& width = _gradients[i].weight;

            ImGui::SetNextItemWidth(std::max(2.f, width * 100));
            if (ImGui::DragFloat(("##colgrad" + std::to_string(i)).c_str(), &_gradients[i].weight, 0.001f, (1.f / 255.f), 1.f))
            {
                _gradients[i].weight = std::min(1.f, _gradients[i].weight);
                _gradients[i].weight = std::max(1.f/255.f, _gradients[i].weight);
                float diff = _gradients[i].weight - oldVal;
                
                if (_gradients.size() > 1)
                {
                    if (i >= 1)
                    {
                        auto& w = _gradients[i - 1].weight;
                        w -= diff;
                        w = std::min(1.f, w);
                        w = std::max(1.f/255.f, w);
                    }
                    else if (i + 1 < _gradients.size()) // i == 0 and has a next gradient to it, subtracts from the right one
                    {
                        {
                            auto& w = _gradients[i + 1].weight;
                            w -= diff;
                            w = std::min(1.f, w);
                            w = std::max(1.f/255.f, w);                        
                        }
                    }
                }
                else
                {
                    _gradients[i].weight = 1.f;
                }
            }
            
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {
                focusedGradient = i;
            }
            
            ImGui::PopStyleColor();

            
            if (i + 1 < _gradients.size())
            {
                ImGui::SameLine();
            }
        
        }
        if (focusedGradient >= 0 && focusedGradient < _gradients.size())
        {
            static bool shouldUnfocus = false;
            if (ImGui::BeginChild("Gradient", {0,100}, true, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::ColorEdit3("Color picker", (float*)&_gradients[focusedGradient].color);

            }

            if (ImGui::Button("Delete"))
            {
                _gradients.erase(_gradients.begin() + focusedGradient);
                    
                if (!_gradients.empty())
                {
                    float sum = 0;

                    for (size_t i = 0; i < _gradients.size(); i++)
                    {
                        sum += _gradients[i].weight;
                    }

                    if (sum != 1.f)
                    {
                        _gradients[_gradients.size() - 1].weight -= sum - 1.f;
                    }

                    shouldUnfocus = true;
                }
            }

            if (shouldUnfocus)
            {
                shouldUnfocus = false;
                focusedGradient = -1;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndChild();
        }
    ImGui::End();
    }
}
*/


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

    _velColors = {
        glm::vec3(68, 1, 84) / glm::vec3(255.f),
        glm::vec3(59, 82, 139) / glm::vec3(255.f),
        glm::vec3(33, 145, 140) / glm::vec3(255.f),
        glm::vec3(94, 201, 98) / glm::vec3(255.f),
        glm::vec3(253,231,37) / glm::vec3(255.f)
    };
    
    glfwSetErrorCallback(&ErrorCallback);

    std::string cutShaderVS = std::string(SHADERS_SRC_LOC) + "cut.vs";
    std::string cutShaderFS = std::string(SHADERS_SRC_LOC) + "cut.fs";
    _cutShader = std::unique_ptr<Shader>(new Shader(cutShaderVS, cutShaderFS));

    glm::uvec3 data_size(500, 100, 500);
    _isabelHgt = vector3d(glm::uvec3(500, 1, 500), 0.0f);
    _isabelTemp = vector3d(data_size, 0.0f);
    _isabelWind = vector3d(data_size, glm::vec3(0));

    std::string hgtFilename = std::string(DATA_LOC) + "HGTdata.bin";
    ReadVolumetricFile(hgtFilename, _isabelHgt);
    std::string tempFilename = std::string(DATA_LOC) + "TCf24.bin";
    ReadVolumetricFile(tempFilename, _isabelTemp);
    char axis[3] = {'U', 'W', 'V'};
    vector3d<float> windAxis(data_size, 0.0f);
    for (int ax = 0; ax < 3; ++ax) {
        std::string windAxisFilename = std::string(DATA_LOC) + axis[ax] + std::string("f24.bin");
        ReadVolumetricFile(windAxisFilename, windAxis);
        for (size_t i = 0; i < windAxis.size(); ++i) {
            if (windAxis[i] > g_invalidValueLimit) {
                _isabelWind[i] = glm::vec3(0);
            } else {
                _isabelWind[i][ax] = windAxis[i];
            }
        }
    }
    for (size_t i = 0; i < _isabelWind.size(); ++i) {
        float vel = glm::length(_isabelWind[i]);
        _maxGlobalVelocity = std::max(vel, _maxGlobalVelocity);
        _minGlobalVelocity = std::min(vel, _minGlobalVelocity);
    }
    for (size_t i = 0; i < _isabelWind.size(); ++i) {
        _isabelWind[i] /= _maxGlobalVelocity;
    }

    // hgt init
    std::string hgtShaderVS = std::string(SHADERS_SRC_LOC) + "hgt.vs";
    std::string hgtShaderFS = std::string(SHADERS_SRC_LOC) + "hgt.fs";
    _hgtShader = std::unique_ptr<Shader>(new Shader(hgtShaderVS, hgtShaderFS));
    _geomGen.HgtGeometry(_isabelHgt, _hgtPts, _hgtIndices);
    VertexLayout hgtVerticesLayout = {
        VertexElement(GL_FLOAT, 3),
        VertexElement(GL_FLOAT, 1),
        VertexElement(GL_FLOAT, 3)
    };
    _hgtVertexBuffer = std::unique_ptr<VertexBuffer>(new VertexBuffer(_hgtPts.size() * sizeof(VertexPosVel), _hgtPts.data(), hgtVerticesLayout));
    _hgtIndexBuffer = std::unique_ptr<IndexBuffer>(new IndexBuffer(_hgtIndices.size() * sizeof(int), _hgtIndices.data()));
    _hgtVertexArray = std::unique_ptr<VertexArray>(new VertexArray(*_hgtVertexBuffer, *_hgtIndexBuffer));
    
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

    _windPts.reserve(data_size.x * data_size.z * 2); // array for uniform lines vbuffer
    _geomGen.GetAxisNumSamples() = _maxAxisNumSamples;
    _geomGen.GenerateGeometry(_isabelWind, Axis::NONE, _cuts[1], _windPts, _windIndices, _genGeomFuncs[static_cast<int>(_geomType)]);

    VertexLayout lineVerticesLayout = {
        VertexElement(GL_FLOAT, 3),
        VertexElement(GL_FLOAT, 1),
        VertexElement(GL_FLOAT, 3)
    };

    _lineVertexBuffer = std::unique_ptr<VertexBuffer>(new VertexBuffer(_windPts.size() * sizeof(VertexPosVel), _windPts.data(), lineVerticesLayout, GL_DYNAMIC_STORAGE_BIT));
    _lineIndexBuffer = std::unique_ptr<IndexBuffer>(new IndexBuffer(_windIndices.size() * sizeof(int), _windIndices.data(), GL_DYNAMIC_STORAGE_BIT));
    _lineVertexArray = std::unique_ptr<VertexArray>(new VertexArray(*_lineVertexBuffer, *_lineIndexBuffer));

    std::shared_ptr<LegendGradient> viridis = std::shared_ptr<LegendGradient>(new LegendGradient());
    _legendGradients.push_back(viridis);
    viridis->gradients.push_back(Gradient(0.f, glm::vec4(68, 1, 84,255) / glm::vec4(255.f)));
    viridis->gradients.push_back(Gradient(0.25f, glm::vec4(59, 82, 139,255) / glm::vec4(255.f)));
    viridis->gradients.push_back(Gradient(0.5f, glm::vec4(33, 145, 140,255) / glm::vec4(255.f)));
    viridis->gradients.push_back(Gradient(0.75f, glm::vec4(94, 201, 98,255) / glm::vec4(255.f)));
    viridis->gradients.push_back(Gradient(1.f, glm::vec4(253,231,37,255) / glm::vec4(255.f)));

    viridis->texture = std::unique_ptr<Texture>(new Texture(GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT));
    std::vector<float> ptrtexdata;
    
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            float id = std::pow((1.f / 200.f) * j, _geomGen.GetSpeedFactor());
            auto col = _legendGradients[0]->ComputeInterpolatedColor(id);
            ptrtexdata.push_back(col.x);
            ptrtexdata.push_back(col.y);
            ptrtexdata.push_back(col.z);
        }
    }
    viridis->texture->SetData2D(200, 10, ptrtexdata.data());

    std::shared_ptr<LegendGradient> temp = std::shared_ptr<LegendGradient>(new LegendGradient());
    _legendGradients.push_back(temp);
    float zeroAt = (1.f / (g_maxTemp - g_minTemp)) * std::abs(g_minTemp);
    temp->gradients.push_back(Gradient(0.f, glm::vec4(0.0, 0.0, 1.0, 1.f)));
    temp->gradients.push_back(Gradient(zeroAt,glm::vec4(1.0)));
    temp->gradients.push_back(Gradient(1.f, glm::vec4(1.0, 0.0, 0.0, 1.f)));

    temp->texture = std::unique_ptr<Texture>(new Texture(GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT));
    std::vector<float> ptrtexdata2;
    
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            float id = (1.f / 200.f) * j;
            auto col = _legendGradients[1]->ComputeInterpolatedColor(id);
            ptrtexdata2.push_back(col.x);
            ptrtexdata2.push_back(col.y);
            ptrtexdata2.push_back(col.z);
        }
    }
    temp->texture->SetData2D(200, 10, ptrtexdata2.data());

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
    if (!ImGui::IsAnyWindowHovered() && !ImGui::IsAnyWindowFocused()) {
        if (_window->IsMouseButtonPressed(MouseButton::LEFT_1)) {
            _camera.GetRotY() -= _camera.GetRotationSpeed() * cursorDir.x;
            _camera.GetRotX() -= _camera.GetRotationSpeed() * cursorDir.y;
        } else if (_window->IsMouseButtonPressed(MouseButton::RIGHT_2)) {
            _camera.Recalculate(aspectRatio);
            glm::mat4 R = _camera.GetRoation();
            glm::vec3 u = R * glm::vec4(1, 0, 0, 1);
            glm::vec3 v = R * glm::vec4(0, 1, 0, 1);
            glm::vec3 diff = -_camera.GetMovementSpeed() * (cursorDir.x * u + cursorDir.y * v);
            _camera.GetCenterPos() = glm::max(glm::vec3(-0.5f), glm::min(glm::vec3(0.5f), _camera.GetCenterPos() + diff));
        }
    }
    _camera.Recalculate(aspectRatio);

    glm::vec3 cameraPosition = _camera.GetV() * glm::vec4(0, 0, 0, 1);

    // draw hgt map
    {
        glm::mat4 PVM = _camera.GetP() * _camera.GetV();

        _hgtShader->SetUniformMat4("u_PVM", PVM);
        _hgtShader->SetUniformFloat3("u_cameraPosition", cameraPosition);
        _hgtShader->SetUniformFloat3("u_lightDir", _lightDir);
        _hgtShader->SetUniformFloat("u_shinines", _shinines);

        _hgtShader->Bind();
        _hgtVertexArray->Bind();
        
        GL(DrawElements(GL_TRIANGLES, _hgtIndices.size(), GL_UNSIGNED_INT, 0));
        
        _hgtShader->Unbind();
        _hgtVertexArray->UnBind();
    }

    // draw temp cuts
    for (int cut = 0; cut < 3; ++ cut) {
        if (!_tempCutEnabled[cut]) {
            continue;
        }
        glm::mat4 M;
        if (cut == 0) {
            M = glm::translate(glm::vec3(_cuts[cut] - 0.5f, 0, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(0, 1, 0));
        }
        if (cut == 1) {
            M = glm::translate(glm::vec3(0, _cuts[cut] - 0.5f, 0)) * glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0));
        }
        if (cut == 2) {
            M = glm::translate(glm::vec3(0, 0, _cuts[cut] - 0.5f));
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
    if (_geomGen.GetVizType() == VizType::CUTS) {
        for (int ax = 0; ax < 3; ++ax)
        {
            if (!_windCutEnabled[ax]) {
                continue;
            }
            _windPts.clear();
            _windIndices.clear();
            _geomGen.GenerateGeometry(_isabelWind, static_cast<Axis>(ax), _cuts[ax], _windPts, _windIndices, _genGeomFuncs[static_cast<int>(_geomType)]);
            _lineVertexBuffer->SetData(0, _windPts.size() * sizeof(VertexPosVel), _windPts.data());
            _lineIndexBuffer->SetData(0, _windIndices.size() * sizeof(int), _windIndices.data());

            glm::mat4 PVM = _camera.GetP() * _camera.GetV();

            _lineShader->SetUniformMat4("u_PVM", PVM);
            _lineShader->SetUniformFloat("u_minVelocity", 0);
            _lineShader->SetUniformFloat("u_maxVelocity", 1);
            
            _lineShader->SetUniformFloat3v("u_colors", _velColors);

            _lineShader->SetUniformFloat3("u_cameraPosition", cameraPosition);
            _lineShader->SetUniformFloat3("u_lightDir", _lightDir);
            _lineShader->SetUniformFloat("u_shinines", _shinines);

            _lineShader->Bind();
            _lineVertexArray->Bind();
            
            GL(DrawElements(GL_TRIANGLES, _windIndices.size(), GL_UNSIGNED_INT, 0));
            
            _lineShader->Unbind();
            _lineVertexArray->UnBind();
        }
    } else {
        _windPts.clear();
        _windIndices.clear();
        _geomGen.GenerateGeometry(_isabelWind, Axis::NONE, 0, _windPts, _windIndices, _genGeomFuncs[static_cast<int>(_geomType)]);
        _lineVertexBuffer->SetData(0, _windPts.size() * sizeof(VertexPosVel), _windPts.data());
        _lineIndexBuffer->SetData(0, _windIndices.size() * sizeof(int), _windIndices.data());

        glm::mat4 PVM = _camera.GetP() * _camera.GetV();

        _lineShader->SetUniformMat4("u_PVM", PVM);
        _lineShader->SetUniformFloat("u_minVelocity", 0);
        _lineShader->SetUniformFloat("u_maxVelocity", 1);
        _lineShader->SetUniformFloat3v("u_colors", _velColors);
        _lineShader->SetUniformFloat3("u_cameraPosition", cameraPosition);
        _lineShader->SetUniformFloat3("u_lightDir", _lightDir);
        _lineShader->SetUniformFloat("u_shinines", _shinines);

        _lineShader->Bind();
        _lineVertexArray->Bind();
        
        //GL(DrawElements(GL_LINES, 500*500*2*3, GL_UNSIGNED_INT, 0));
        GL(DrawElements(GL_TRIANGLES, _windIndices.size(), GL_UNSIGNED_INT, 0));
        
        _lineShader->Unbind();
        _lineVertexArray->UnBind();
    }
    
    
    {
        ImGuiDraw imguiDraw = _imguiContext->Draw();

        ImGui::Begin("Settings");
        
        if (ImGui::BeginTabBar("##TabBar"))
        {
            if (ImGui::BeginTabItem("Main")) {
                bool resetCamPos = ImGui::Button("Reset camera pos");
                if (resetCamPos) {
                    _camera.GetCenterPos() = glm::vec3(0);
                }
                if (ImGui::CollapsingHeader("Cut positions", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::SliderFloat("X cut pos", &_cuts[0], 0, 1);
                    ImGui::SliderFloat("Y cut pos", &_cuts[1], 0.005f, 1);
                    ImGui::SliderFloat("Z cut pos", &_cuts[2], 0, 1);
                }
                if (ImGui::CollapsingHeader("Temperature", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::Checkbox("Temperature X Cut", &_tempCutEnabled[0]);
                    ImGui::Checkbox("Temperature Y Cut", &_tempCutEnabled[1]);
                    ImGui::Checkbox("Temperature Z Cut", &_tempCutEnabled[2]);
                }
                if (ImGui::CollapsingHeader("Wind", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    static const char* windVizType[] = { "selection", "cuts" };
                    ImGui::Combo("Visualization type", (int*)&_geomGen.GetVizType(), windVizType, IM_ARRAYSIZE(windVizType));
                    if (_geomGen.GetVizType() == VizType::CUTS) {
                        ImGui::Checkbox("Wind X Cut", &_windCutEnabled[0]);
                        ImGui::Checkbox("Wind Y Cut", &_windCutEnabled[1]);
                        ImGui::Checkbox("Wind Z Cut", &_windCutEnabled[2]);
                    } else {
                        ImGui::SliderFloat3("Selection pos", (float*)&_geomGen.GetSelectionPos(), 0.0f, 1.0f);
                        ImGui::SliderFloat("Selection size", &_geomGen.GetSelectionSize(), 0.01f, 1);
                    }

                    static const char* geomType[] = { "streamlines", "arrows"};
                    ImGui::Combo("Geometry type", (int*)&_geomType, geomType, IM_ARRAYSIZE(geomType));
                    ImGui::Text("Samples");
                    ImGui::SliderInt("Num. samples axis", (int*)&_geomGen.GetAxisNumSamples(), _minAxisNumSamples, _maxAxisNumSamples);
                    ImGui::Checkbox("Use random samples", &_geomGen.GetUseRandomSamples());
                    ImGui::Text("Segments");
                    ImGui::SliderInt("Streamline num. segments", (int*)&_geomGen.GetStreamlineSegments(), 1, 20);
                    ImGui::SliderInt("Streamline scale", (int*)&_geomGen.GetStreamlineScale(), 1, 10);
                    ImGui::Text("Arrows");
                    ImGui::Checkbox("Scale arrows", &_geomGen.GetScaleArrows());
                }

                {
                    ImGui::Text("Wind velocity (m/s)");
                    ImGui::Image((void*)(intptr_t)_legendGradients[0]->texture->GetID(), {200,10}, {0,0}, {1,1});
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip("Endpoint color denotes the maximal wind velocity of the arrow/streamline. Also denotes the streamline orientation.");
                    }
                    ImGui::Text(std::to_string((int)_minGlobalVelocity).c_str());
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(200);
                    ImGui::Text(std::to_string((int)_maxGlobalVelocity).c_str());
                }
                {
                    ImGui::Text("Air temperature (°C)");
                    ImGui::Image((void*)(intptr_t)_legendGradients[1]->texture->GetID(), {200,10}, {0,0}, {1,1});
                    ImGui::Text(std::to_string((int)g_minTemp).c_str());
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(_legendGradients[1]->gradients[1].weight * 200);
                    ImGui::Text(std::to_string(0).c_str());
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(_legendGradients[1]->gradients[2].weight * 200);
                    ImGui::Text(std::to_string((int)g_maxTemp).c_str());
                }
                

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Other")) {
                ImGui::Text("Cut colors");
                ImGui::ColorEdit3("cold color", &_coldColor[0]);
                ImGui::ColorEdit3("zero color", &_zeroColor[0]);
                ImGui::ColorEdit3("warm color", &_warmColor[0]);
                ImGui::ColorEdit3("invalid color", &_invalidColor[0]);
                ImGui::Text("Speed colors");
                for (int c = 0; c < (int)_velColors.size(); ++c) {
                    ImGui::ColorEdit3((std::string("speed color ") + std::to_string(c)).c_str(), (float*)&_velColors[c]);
                }
                
                ImGui::Text("Camera");
                ImGui::SliderFloat3("Cam pos", (float*)&_camera.GetCenterPos(), -0.5f, 0.5f);
                ImGui::SliderFloat("rot y", &_camera.GetRotY(), -180, 180);
                ImGui::SliderFloat("rot x", &_camera.GetRotX(), -89, 89);
                ImGui::SliderFloat("cam move speed", &_camera.GetMovementSpeed(), 0, 1.5f);
                ImGui::SliderFloat("cam rot speed", &_camera.GetRotationSpeed(), 0, 500);
                ImGui::SliderFloat("cam FOV", &_camera.GetFOV(), _minFOV, _maxFOV);

                ImGui::Text("Light");
                ImGui::SliderFloat("shinines", &_shinines, 1, 100);

                ImGui::Text("Streamline speed");
                
                if (ImGui::SliderFloat("Speed factor", &_geomGen.GetSpeedFactor(), 0.1, 1))
                {
                    std::vector<float> ptrtexdata;
                    
                    for (int i = 0; i < 10; i++)
                    {
                        for (int j = 0; j < 200; j++)
                        {
                            float id = std::pow((1.f / 200.f) * j, _geomGen.GetSpeedFactor());
                            auto col = _legendGradients[0]->ComputeInterpolatedColor(id);
                            ptrtexdata.push_back(col.x);
                            ptrtexdata.push_back(col.y);
                            ptrtexdata.push_back(col.z);
                        }
                    }
                    _legendGradients[0]->texture->SetData2D(200, 10, ptrtexdata.data());
                }
                
                {
                    ImGui::Text("Wind velocity (m/s)");
                    ImGui::Image((void*)(intptr_t)_legendGradients[0]->texture->GetID(), {200,10}, {0,0}, {1,1});
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip("Denotes the streamline orientation. Endpoint color denotes the maximal wind velocity of the streamline.");
                    }
                    //ImGui::SetCursorPosX(0);
                    ImGui::Text(std::to_string((int)_minGlobalVelocity).c_str());
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(200);
                    ImGui::Text(std::to_string((int)_maxGlobalVelocity).c_str());
                    //ImGui::Text("Maximal velocity");
                    //ImGui::
                }
                {
                    ImGui::Text("Air temperature (°C)");
                    ImGui::Image((void*)(intptr_t)_legendGradients[1]->texture->GetID(), {200,10}, {0,0}, {1,1});
                    //ImGui::SetCursorPosX(_legendGradients[1]->gradients[0].weight * 200);
                    ImGui::Text(std::to_string((int)g_minTemp).c_str());
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(_legendGradients[1]->gradients[1].weight * 200);
                    ImGui::Text(std::to_string(0).c_str());
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(_legendGradients[1]->gradients[2].weight * 200);
                    ImGui::Text(std::to_string((int)g_maxTemp).c_str());
                }
                
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