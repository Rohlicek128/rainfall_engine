#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


class IGui
{
public:
    virtual ~IGui() = default;
    virtual void set_gui() = 0;
};
