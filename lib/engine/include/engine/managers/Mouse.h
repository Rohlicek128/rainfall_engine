#pragma once

#include <unordered_set>


namespace engine
{
    class Window;

    class Mouse
    {
        Window* window_;
        bool is_visible_;

        std::unordered_set<int> pressed_buttons_;

    public:
        float last_x, last_y;
        float pos_x, pos_y;
        float sensitivity;

        float scroll_x_delta, scroll_y_delta;

        bool first_move;

        Mouse(Window& window, float sensitivity, bool visible = true);
        ~Mouse() = default;

        void set_visibility(bool visible);
        bool is_visible();

        bool moved();

        void update_buttons(int* button, int* action);
        bool get_button_up(int button);
        bool get_button_down(int button);

        bool is_scrolling_x();
        bool is_scrolling_y();
    };
}
