#pragma once

#include "common.h"

enum class MouseButton : int
{
    LEFT_1,
    RIGHT_2,
    MIDDLE_3,
    OTHER_4,
    OTHER_5,
    OTHER_6,
    OTHER_7,
    OTHER_8
};

class Window
{
private:
    GLFWwindow* _id;
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    glm::ivec2 GetSize() const;
	void SetSize(int width, int height);

    void SwapBuffers();
    bool ShouldClose() const;

    GLFWwindow* GetID() const { return _id; };
    
    glm::vec2 GetCursorPosScreen() const;
    glm::vec2 GetCursorPosEye() const;
    glm::vec2 ScreenPosToEyePos(glm::vec2 screenPos) const;
    bool IsMouseButtonPressed(MouseButton button) const;
};