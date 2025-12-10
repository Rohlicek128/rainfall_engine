#pragma once

#include <glad.h>
#include <glfw3.h>

#include <string>


class Scene;

namespace engine
{
    class EngineArgs;

    class Window
    {
        GLFWwindow* init_window(const char* window_name, const int width, const int height);

        static void framebuffer_size_callback(GLFWwindow*, int width, int height);
        static void mouse_callback(GLFWwindow*, double x, double y);
        static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    public:
        static EngineArgs engine_args;

        Window(const std::string&, int, int);
        ~Window();

        GLFWwindow* get_glfw_window();
        bool should_close();

        bool set_icon(const std::string& path);
    };
}
