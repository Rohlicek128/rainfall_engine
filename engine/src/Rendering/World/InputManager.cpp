#include "engine/managers/InputManager.h"
#include <memory>

#include "engine/rendering/Window.h"
#include "engine/rendering/EngineArgs.h"

namespace engine
{
    InputManager::InputManager(Window& window)
    {
        window_ = &window;
        current_delta_time_ = 0;

        mouse = std::make_unique<Mouse>(window, 5.0f, false);
    }

    void InputManager::update(float delta_time)
    {
        current_delta_time_ = delta_time;

        mouse->pos_x = window_->engine_args.mouse_x;
        mouse->pos_y = window_->engine_args.mouse_y;
        mouse->scroll_x_delta = window_->engine_args.scroll_x;
        mouse->scroll_y_delta = window_->engine_args.scroll_y;
        mouse->update_buttons(&window_->engine_args.mouse_button, &window_->engine_args.mouse_action);


        //END
        if (get_key_with_timeout(GLFW_KEY_END, 1000))
            glfwSetWindowShouldClose(window_->engine_args.window, true);

        //ESCAPE
        if (get_key_toggle(GLFW_KEY_ESCAPE))
        {
            mouse->set_visibility(!mouse->is_visible());
            mouse->first_move = true;
        }

        //F11
        if (get_key_toggle(GLFW_KEY_F11))
        {
            is_fullscreen_ = !is_fullscreen_;
            if (is_fullscreen_)
                glfwSetWindowMonitor(window_->engine_args.window, glfwGetPrimaryMonitor(), 50, 50, window_->engine_args.width, window_->engine_args.height, GLFW_DONT_CARE);
            else
                glfwSetWindowMonitor(window_->engine_args.window, nullptr, 50, 50, window_->engine_args.width, window_->engine_args.height, GLFW_DONT_CARE);
        }


        window_->engine_args.scroll_x = 0.0f;
        window_->engine_args.scroll_y = 0.0f;
    }


    bool InputManager::get_key_up(int key)
    {
        return glfwGetKey(window_->get_glfw_window(), key) == GLFW_RELEASE;
    }

    bool InputManager::get_key_down(int key)
    {
        return glfwGetKey(window_->get_glfw_window(), key) == GLFW_PRESS;
    }

    bool InputManager::get_key_toggle(int key)
    {
        int key_state = glfwGetKey(window_->get_glfw_window(), key);
        if (key_state == GLFW_PRESS && toggled_keys_.find(key) == toggled_keys_.end())
        {
            toggled_keys_.insert(key);
            return true;
        }
        else if (key_state == GLFW_RELEASE && toggled_keys_.find(key) != toggled_keys_.end())
            toggled_keys_.erase(key);

        return false;
    }

    bool InputManager::get_key_with_timeout(int key, int timeout_ms)
    {
        bool pressed = glfwGetKey(window_->get_glfw_window(), key) == GLFW_PRESS;
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
