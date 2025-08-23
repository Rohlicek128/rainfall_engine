#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Rendering/Engine.h"

EngineArgs render_args;

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
    render_args.width = width;
    render_args.height = height;
}

void mouse_callback(GLFWwindow*, double x, double y)
{
    render_args.mouse_x = static_cast<float>(x);
    render_args.mouse_y = static_cast<float>(y);
}

GLFWwindow* init_window(const int width, const int height, const char* window_name)
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


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw "Initialize GLAD: FAILED";
    }
    
    glfwSetInputMode(window, GLFW_CURSOR,  GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glViewport(0, 0, width, height);
    render_args.width = width;
    render_args.height = height;

    glfwSetWindowPos(window, 50, 50);

    return window;
}

int main()
{
    GLFWwindow* window;
    try
    {
        window = init_window(1920, 1080, "RAINFALL ENGINE");
    }
    catch (std::string e)
    {
        std::cout << e << '\n';
        glfwTerminate();
        return -1;
    }
    render_args.window = window;
    
    Engine engine(render_args);
    while (!glfwWindowShouldClose(window))
    {
        engine.update(render_args);
        engine.render(render_args);
    }
    
    return 0;
}