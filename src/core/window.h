#pragma once

#include "common.h"

class Window
{
private:
    GLFWwindow* _id;
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    void GetSize(int& width, int& height) const;
	void SetSize(int width, int height);

    void SwapBuffers();
    bool ShouldClose() const;

    GLFWwindow* GetID() { return _id; };
};