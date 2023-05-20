#include "viz_app.h"

float g_triangle[] = {
   -0.7, -0.7, 0.0,
   0.0, 0.7, 0.0,
   0.7, -0.7, 0.0,
};
int g_triangleIndices[] = {0, 1, 2};

std::string g_vertSrc = R"(
    #version 330 core
			
    layout(location = 0) in vec3 a_position;

    void main()
    {
        gl_Position = vec4(a_position, 1.0);	
    }
)";
std::string g_fragSrc = R"(
    #version 330 core
			
    layout(location = 0) out vec4 color;

    void main()
    {
        color = vec4(1.0, 1.0, 1.0, 1.0);
    }
)";

void VizApp::Init(Window* window, VizImGuiContext* imguiContext)
{
    _window = window;
    _imguiContext = imguiContext;
    _basicShader = Shader::CreateFromSource(g_vertSrc, g_fragSrc);
    
    _triangleVertexBuffer = std::unique_ptr<VertexBuffer>(new VertexBuffer(3 * 3 * sizeof(float), g_triangle, VertexLayout({VertexElement(GL_FLOAT, 3)})));
    _triangleIndexBuffer = std::unique_ptr<IndexBuffer>(new IndexBuffer(3 * sizeof(int), g_triangleIndices));
    _triangleVertexArray = std::unique_ptr<VertexArray>(new VertexArray(*_triangleVertexBuffer, *_triangleIndexBuffer));

    GL(ClearColor(0, 0, 0, 1));
}

void VizApp::Update(double deltaTime)
{
    glm::ivec2 winSize = _window->GetSize();
    GL(Viewport(0, 0, winSize.x, winSize.y));
    
    GL(Clear(GL_COLOR_BUFFER_BIT));

    _basicShader->Bind();
    _triangleVertexArray->Bind();

    GL(DrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
    //GL(DrawArrays(GL_TRIANGLES, 0, 3));

    
    _basicShader->Unbind();
    _triangleVertexArray->UnBind();

    {
        ImGuiDraw imguiDraw = _imguiContext->Draw();

        ImGui::Begin("Test");

        ImGui::Text("test text");

        ImGui::End();
    }
}