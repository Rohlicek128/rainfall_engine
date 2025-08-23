#pragma once
#include "../../Imgui/imgui.h"
#include "../../Imgui/imgui_impl_glfw.h"
#include "../../Imgui/imgui_impl_opengl3.h"


class IGui
{
public:
    virtual ~IGui() = default;
    virtual void set_gui() = 0;
};
