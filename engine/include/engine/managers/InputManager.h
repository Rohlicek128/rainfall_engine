#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Mouse.h"


namespace engine
{
    class Window;

    class InputManager
    {
        Window* window_;
        float current_delta_time_;

        std::unordered_set<int> toggled_keys_;
        std::unordered_map<int, float> timeout_keys_;

        bool is_fullscreen_;

    public:
        std::unique_ptr<Mouse> mouse;

        InputManager(Window& window);

        void update(float delta_time);

        bool get_key_up(int key);
        bool get_key_down(int key);
        bool get_key_toggle(int key);
        bool get_key_with_timeout(int key, int timeout_ms);
    };
}
