#pragma once

#include "imgui.h"


namespace engine
{
    class Renderer;
}

namespace editor
{
    class PerformancePanel
    {
    public:
        void draw(ImVec2 pos, engine::Renderer& renderer);
    };
}
