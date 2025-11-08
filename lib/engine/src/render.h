#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace engine
{
    class Renderer
    {
        int frame;
        GLFWwindow* _window;

    public:
        Renderer();

        void terminate();

        GLFWwindow* init_window(const char* window_name, const int width, const int height);
        void render();
        int get_frame_count();
    };
}
