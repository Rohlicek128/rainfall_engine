#pragma once

#include "imgui.h"

namespace engine
{
    class Window;
}

namespace editor
{
    class TitlebarPanel
    {
    public:
        bool titleBarHovered;

        TitlebarPanel() = default;
        void draw(engine::Window& window);
    };
}
