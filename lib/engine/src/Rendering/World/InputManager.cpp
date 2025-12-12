#include "engine/managers/InputManager.h"
#include "glfw3.h"

namespace engine
{
    InputManager::InputManager(GLFWwindow* window)
    {
        window_ = window;
        current_delta_time_ = 0;
    }

    void InputManager::set_delta_time(float delta_time)
    {
        current_delta_time_ = delta_time;
    }


    bool InputManager::get_key_up(int key)
    {
        return glfwGetKey(window_, key) == GLFW_RELEASE;
    }

    bool InputManager::get_key_down(int key)
    {
        return glfwGetKey(window_, key) == GLFW_PRESS;
    }

    bool InputManager::get_key_toggle(int key)
    {
        if (glfwGetKey(window_, key) == GLFW_PRESS && toggled_keys_.find(key) == toggled_keys_.end())
        {
            toggled_keys_.insert(key);
            return true;
        }
        else if (glfwGetKey(window_, key) == GLFW_RELEASE && toggled_keys_.find(key) != toggled_keys_.end())
            toggled_keys_.erase(key);

        return false;
    }

    bool InputManager::get_key_with_timeout(int key, int timeout_ms)
    {
        bool pressed = glfwGetKey(window_, key);
        if (timeout_keys_.find(key) == timeout_keys_.end())
        {
            if (pressed) timeout_keys_.emplace(key, 0.0f);
            return pressed;
        }

        timeout_keys_.at(key) += current_delta_time_ * 1000.0f;
        if (timeout_keys_.at(key) < timeout_ms)
            return false;

        timeout_keys_.erase(key);
        if (pressed) timeout_keys_.emplace(key, 0.0f);
        return pressed;
    }
}
