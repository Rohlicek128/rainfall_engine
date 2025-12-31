#pragma once

#include <memory>
#include <engine/core/Application.h>

#include "gui/panels/ViewportPanel.h"
#include "gui/panels/PerformancePanel.h"
#include "gui/panels/ProjectsModalPanel.h"
#include "gui/panels/SceneGraphPanel.h"
#include "gui/panels/EntityInspectorPanel.h"


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
        std::unique_ptr<ViewportPanel> viewport_panel_;
        std::unique_ptr<PerformancePanel> performance_panel_;
        std::unique_ptr<ProjectsModalPanel> project_modal_;
        std::unique_ptr<SceneGraphPanel> scene_graph_panel_;
        std::unique_ptr<EntityInspectorPanel> entity_inspector_panel_;

        bool show_imgui_demo_;
        bool show_projects_modal_;

        Scene* create_sample_scene();
        void draw_dockspace();
    public:
        EditorApplication();

        void on_start() override;
        void on_update(const float delta_time) override;
        void on_render(engine::Renderer& renderer);
    };
}
