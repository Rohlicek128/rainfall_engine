#pragma once

#include <glfw3.h>
#include <unordered_map>
#include <unordered_set>


namespace engine
{
    class InputManager
    {
        GLFWwindow* window_;
        float current_delta_time_;

        std::unordered_set<int> toggled_keys_;
        std::unordered_map<int, float> timeout_keys_;

    public:
        InputManager(GLFWwindow* window);

        void set_delta_time(float delta_time);

        bool get_key_up(int key);
        bool get_key_down(int key);
        bool get_key_toggle(int key);
        bool get_key_with_timeout(int key, int timeout_ms);
    };
}
