#pragma once
#include "../Gui/IGui.h"

class Mouse : public IGui
{
public:
    float last_x, last_y;
    float pos_x, pos_y;
    float sensitivity;

    bool first_move;
    bool is_visible;
    
    Mouse(int, int, float);
    ~Mouse() override = default;
    void set_gui() override;
};
