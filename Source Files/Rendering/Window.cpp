#include "Window.h"

EngineArgs Window::engine_args;

Window::Window(const std::string& name, const int width, const int height)
{
    GLFWwindow* window;
    try
    {
        window = init_window(name.c_str(), width, height);
    }
    catch (std::string e)
    {
        std::cout << e << '\n';
        glfwTerminate();
        exit(-1);
    }
    engine_args.window = window;
    is_running_ = false;
}

Window::~Window()
{
    glfwTerminate();
}

void Window::run(Engine& engine)
{
    is_running_ = true;
    while (is_running_ && !glfwWindowShouldClose(engine_args.window))
    {
        engine.update(engine_args);
        engine.render(engine_args);
    }
}

void Window::stop()
{
    is_running_ = false;
}

GLFWwindow* Window::init_window(const char* window_name, const int width, const int height)
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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glViewport(0, 0, width, height);
    engine_args.width = width;
    engine_args.height = height;

    glfwSetWindowPos(window, 50, 50);

    return window;
}

void Window::framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
    engine_args.width = width;
    engine_args.height = height;
}

void Window::mouse_callback(GLFWwindow*, double x, double y)
{
    engine_args.mouse_x = static_cast<float>(x);
    engine_args.mouse_y = static_cast<float>(y);
}

void Window::mouse_scroll_callback(GLFWwindow*, double xoffset, double yoffset)
{
    engine_args.scroll_x = static_cast<float>(xoffset);
    engine_args.scroll_y = static_cast<float>(yoffset);
}