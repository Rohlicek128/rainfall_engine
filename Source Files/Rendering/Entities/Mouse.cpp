#include "Mouse.h"

#include <string>

Mouse::Mouse(const int width, const int height, float sens)
{
    last_x = width / 2;
    last_y = height / 2;
    pos_x = last_x;
    pos_y = last_y;
    sensitivity = sens;

    first_move = true;
    is_visible = false;
}

void Mouse::set_gui()
{
    ImGui::Begin("Mouse");

    std::string str = "Screen: (X: " + std::to_string(pos_x) + ", Y:" + std::to_string(pos_y) + ")";
    ImGui::Text(str.c_str());

    ImGui::SliderFloat("Sensitivity", &sensitivity, 1.0f, 100.0f);

    ImGui::End();
}
