#pragma once

#include "imgui.h"


namespace engine
{
    class Application;
}

namespace editor
{
    class ProjectsModalPanel
    {
    public:
        void draw(engine::Application& app, bool* open, bool* open_new);
    };
}
