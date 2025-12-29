#pragma once

#include <memory>
#include <engine/core/Application.h>
#include "gui/panels/ViewportPanel.h"
#include "gui/panels/PerformancePanel.h"


class ImGuiIO;
class Framebuffer;

namespace engine
{
    class Renderer;
}

namespace editor
{
    class EditorApplication : public engine::Application
    {
        std::string working_dir_;

        std::unique_ptr<ViewportPanel> viewport_panel_;
        std::unique_ptr<PerformancePanel> performance_panel_;

        void draw_dockspace();

    public:
        void on_start() override;
        void on_update(const float delta_time) override;
        void on_render(engine::Renderer& renderer);
    };
}
