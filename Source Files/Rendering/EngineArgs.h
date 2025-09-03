#pragma once

#include "GLFW/glfw3.h"

struct EngineArgs
{
    GLFWwindow* window;
    float mouse_x, mouse_y;
    float scroll_x, scroll_y;
    int width, height;
};
