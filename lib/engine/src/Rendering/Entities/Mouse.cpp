#include "Mouse.h"

#include <string>

Mouse::Mouse(const int width, const int height, const float sens, const bool visible)
{
    last_x = width / 2;
    last_y = height / 2;
    pos_x = last_x;
    pos_y = last_y;
    sensitivity = sens;

    first_move = true;
    is_visible = visible;
}

void Mouse::set_gui()
{
    ImGui::Begin("Mouse");

    std::string str = "Screen: (X: " + std::to_string(pos_x) + ", Y:" + std::to_string(pos_y) + ")";
    ImGui::Text(str.c_str());

    ImGui::SliderFloat("Sensitivity", &sensitivity, 1.0f, 100.0f);

    ImGui::End();
}
