#pragma once

#include <core/common.h>
#include <core/window.h>

#include <imgui/imgui.h>

class VizImGuiContext;

class ImGuiDraw
{
    friend class VizImGuiContext;
protected:
    const Window* _window = nullptr;
protected:
    ImGuiDraw(const Window& window);
public:
    ImGuiDraw() {}
    ImGuiDraw(const ImGuiDraw& draw) = delete;
    ImGuiDraw(ImGuiDraw&& draw);
    ImGuiDraw& operator=(ImGuiDraw&& draw);
    virtual ~ImGuiDraw();
};

class VizImGuiContext
{
protected:
    const Window* _window = nullptr;
public:
    VizImGuiContext(const Window& window);
    virtual ~VizImGuiContext();
    ImGuiDraw Draw();
};