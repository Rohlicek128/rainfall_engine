#include "engine/core/Application.h"
#include "engine/managers/InputManager.h"
#include "engine/managers/ResourceManager.h"
#include "engine/managers/SceneManager.h"
#include "glfw3.h"
#include <memory>

namespace engine
{
    Application::Application()
    {
        current_project = std::make_unique<Project>();

        resource_manager = std::make_unique<ResourceManager>();
        scene_manager = std::make_unique<SceneManager>(*resource_manager);
        input_manager = nullptr;
    }

    void Application::set_window(GLFWwindow* window)
    {
        input_manager = std::make_unique<InputManager>(window);
    }

    void Application::reset()
    {
        resource_manager->reset();
        scene_manager->reset();
    }

    bool Application::is_running()
    {
        return is_running_;
    }

    void Application::stop()
    {
        is_running_ = false;
    }

    float Application::get_uptime()
    {
        return glfwGetTime();
    }
}
