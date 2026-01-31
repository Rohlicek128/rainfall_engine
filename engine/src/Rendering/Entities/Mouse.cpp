#include "engine/managers/Mouse.h"

#include "engine/rendering/Window.h"


namespace engine
{
    Mouse::Mouse(Window& window, const float sens, const bool visible)
    {
        window_ = &window;

        last_x = window_->get_width() / 2;
        last_y = window_->get_height() / 2;
        pos_x = last_x;
        pos_y = last_y;
        sensitivity = sens;

        scroll_x_delta = 0;
        scroll_y_delta = 0;

        first_move = true;
        set_visibility(visible);
    }


    void Mouse::set_visibility(bool visible)
    {
        is_visible_ = visible;

        if (is_visible_)
        {
            glfwSetInputMode(window_->get_glfw_window(), GLFW_CURSOR,  GLFW_CURSOR_NORMAL);
            //editor_->imgui_io->ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        }
        else
        {
            glfwSetInputMode(window_->get_glfw_window(), GLFW_CURSOR,  GLFW_CURSOR_DISABLED);
            //editor_->imgui_io->ConfigFlags |= ImGuiConfigFlags_NoMouse;
        }
    }

    bool Mouse::is_visible()
    {
        return is_visible_;
    }

    bool Mouse::moved()
    {
        return pos_x != last_x || pos_y != last_y;
    }


    void Mouse::update_buttons(int* button, int* action)
    {
        if (*button == -1 || *action == -1) return;

        if (*action == GLFW_PRESS && pressed_buttons_.find(*button) == pressed_buttons_.end())
            pressed_buttons_.insert(*button);
        else if (*action == GLFW_RELEASE && pressed_buttons_.find(*button) != pressed_buttons_.end())
            pressed_buttons_.erase(*button);

        *button = -1;
        *action = -1;
    }

    bool Mouse::get_button_up(int button)
    {
        return pressed_buttons_.find(button) == pressed_buttons_.end();
    }

    bool Mouse::get_button_down(int button)
    {
        return pressed_buttons_.find(button) != pressed_buttons_.end();
    }


    bool Mouse::is_scrolling_x()
    {
        return std::fabs(scroll_x_delta) > 0.001f;
    }

    bool Mouse::is_scrolling_y()
    {
        return std::fabs(scroll_y_delta) > 0.001f;
    }

}
