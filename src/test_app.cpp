#include "test_app.h"

float g_triangle[] = {
   -0.7, -0.7, 0.0,
   0.0, 0.7, 0.0,
   0.7, -0.7, 0.0,
};
int g_triangleIndices[] = {0, 1, 2};

uint8_t g_texture[] = {
    255, 0, 0,
    0, 255, 0,
    0, 0, 255,
    255, 255, 255
};

void TestApp::Init(Window* window, VizImGuiContext* imguiContext)
{
    _window = window;
    _imguiContext = imguiContext;
    std::string testShaderVS = std::string(SHADERS_SRC_LOC) + "test.vs";
    std::string testShaderFS = std::string(SHADERS_SRC_LOC) + "test.fs";
    _basicShader = std::unique_ptr<Shader>(new Shader(testShaderVS, testShaderFS));
    
    _triangleVertexBuffer = std::unique_ptr<VertexBuffer>(new VertexBuffer(3 * 3 * sizeof(float), g_triangle, VertexLayout({VertexElement(GL_FLOAT, 3)})));
    _triangleIndexBuffer = std::unique_ptr<IndexBuffer>(new IndexBuffer(3 * sizeof(int), g_triangleIndices));
    _triangleVertexArray = std::unique_ptr<VertexArray>(new VertexArray(*_triangleVertexBuffer, *_triangleIndexBuffer));
    _texture = std::unique_ptr<Texture>(new Texture(GL_TEXTURE_2D, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE));
    _texture->SetData2D(2, 2, g_texture);

    GL(ClearColor(0, 0, 0, 1));
}

void TestApp::Update(double deltaTime)
{
    glm::ivec2 winSize = _window->GetSize();
    GL(Viewport(0, 0, winSize.x, winSize.y));
    
    GL(Clear(GL_COLOR_BUFFER_BIT));
    
    _basicShader->SetUniformFloat3("u_color", glm::vec3(1, 0.5, .05));
    _basicShader->SetUniformInt("u_textureSampler", 0);

    _basicShader->Bind();
    _texture->BindToUnit(0);
    _triangleVertexArray->Bind();

    GL(DrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
    //GL(DrawArrays(GL_TRIANGLES, 0, 3));

    
    _basicShader->Unbind();
    _triangleVertexArray->UnBind();

    {
        ImGuiDraw imguiDraw = _imguiContext->Draw();

        ImGui::Begin("Test");

        ImGui::Text("test text");

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", deltaTime * 1000.0, 1.0 / deltaTime);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::End();
    }
}