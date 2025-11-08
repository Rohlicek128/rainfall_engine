#include "render.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "GLFW/glfw3.h"

#include <iostream>


namespace engine
{
    Renderer::Renderer() : frame(0)
    {
        _window = nullptr;
    }

    void Renderer::terminate()
    {
        glfwTerminate();
    }

    GLFWwindow* Renderer::init_window(const char* window_name, const int width, const int height)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        GLFWwindow* window = glfwCreateWindow(width, height, window_name, nullptr, nullptr);
        if (window == nullptr)
        {
            throw "Create GLFW Window: FAILED";
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);


        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw "Initialize GLAD: FAILED";
        }
        glViewport(0, 0, width, height);

        glfwSetWindowPos(window, 50, 50);

        _window = window;
        return _window;
    }

    void Renderer::render()
    {
        std::cout << "RENDERING\n";
        frame++;
    }

    int Renderer::get_frame_count()
    {
        return frame;
    }
}
