#pragma once

#include "imgui.h"


namespace engine
{
    class Renderer;
}

namespace editor
{
    class ViewportPanel
    {
    public:
        ImVec2 pos, size;

        ViewportPanel() = default;
        void draw(engine::Renderer& renderer);
    };
}
