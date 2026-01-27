#pragma once

#include <RmlUi/Core.h>
#include <glfw.h>

namespace engine
{
    class SystemInterface_GLFW : Rml::SystemInterface
    {
    public:
        double GetElapsedTime() override { return glfwGetTime(); }
    };
}