#pragma once

#include "imgui.h"


class Framebuffer;

namespace editor
{
    class ViewportPanel
    {
    public:
        ImVec2 pos, size;

        ViewportPanel() = default;
        void draw(Framebuffer& fbo);
    };
}
