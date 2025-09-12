#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Engine.h"
#include "EngineArgs.h"

class Window
{
    bool is_running_;

    GLFWwindow* init_window(const char* window_name, const int width, const int height);

    static void framebuffer_size_callback(GLFWwindow*, int width, int height);
    static void mouse_callback(GLFWwindow*, double x, double y);
    static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
public:
    static EngineArgs engine_args;

    Window(const std::string&, int, int);
    ~Window();
    
    void run(Engine&);
    void stop();
};
