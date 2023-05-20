#include "imgui_context.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

ImGuiDraw::ImGuiDraw(const Window& window) : _window(&window)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
ImGuiDraw::ImGuiDraw(ImGuiDraw&& draw)
{
    std::swap(_window, draw._window);
}
ImGuiDraw& ImGuiDraw::operator=(ImGuiDraw&& draw)
{
    std::swap(_window, draw._window);
    return *this;
}
ImGuiDraw::~ImGuiDraw()
{
    ImGuiIO& io = ImGui::GetIO();
    glm::ivec2 winSize = _window->GetSize();
    io.DisplaySize = ImVec2(winSize.x, winSize.y);

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

VizImGuiContext::VizImGuiContext(const Window& window) : _window(&window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; 

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    //style.Alpha = 0.9f;

    ImGui_ImplGlfw_InitForOpenGL(window.GetID(), true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

VizImGuiContext::~VizImGuiContext()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

ImGuiDraw VizImGuiContext::Draw()
{
    return ImGuiDraw(*_window);
}