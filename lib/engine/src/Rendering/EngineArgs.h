#pragma once

#include <glfw3.h>


namespace engine
{
    struct EngineArgs
    {
        GLFWwindow* window;
        float mouse_x, mouse_y;
        float scroll_x, scroll_y;
        int width, height;
    };
}
