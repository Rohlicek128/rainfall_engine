#pragma once

#include "imgui.h"


namespace editor
{
    class EditorApplication;

    class ProjectNewModal
    {
    public:
        void draw(EditorApplication& app, bool* open);
    };
}
