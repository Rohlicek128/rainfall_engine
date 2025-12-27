#pragma once

class Mouse
{
public:
    float last_x, last_y;
    float pos_x, pos_y;
    float sensitivity;

    bool first_move;
    bool is_visible;

    Mouse(int, int, float, bool = true);
    ~Mouse() = default;
};
