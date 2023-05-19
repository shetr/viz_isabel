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

void VizApp::Init()
{
    _basicShader = std::unique_ptr<Shader>(new Shader(true, g_vertSrc, g_fragSrc));
    
    _triangleVertexBuffer = std::unique_ptr<VertexBuffer>(new VertexBuffer(3, g_triangle, VertexLayout({VertexElement(GL_FLOAT, 3)})));
    _triangleIndexBuffer = std::unique_ptr<IndexBuffer>(new IndexBuffer(3, g_triangleIndices));
    _triangleVertexArray = std::unique_ptr<VertexArray>(new VertexArray(*_triangleVertexBuffer, *_triangleIndexBuffer));

    GL(ClearColor(0, 0, 0, 1));
}

void VizApp::Update()
{
    GL(Clear(GL_COLOR_BUFFER_BIT));

    _basicShader->Bind();
    _triangleVertexArray->Bind();

    GL(DrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
    //GL(DrawArrays(GL_TRIANGLES, 0, 3));

    
    _basicShader->Unbind();
    _triangleVertexArray->UnBind();

    //glBegin(GL_TRIANGLES);
    //    glVertex3f(-0.7, -0.7, 0.0);
    //    glVertex3f(0.0, 0.7, 0.0);
    //    glVertex3f(0.7, -0.7, 0.0);
    //glEnd();
    //glFlush();
}