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
