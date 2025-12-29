#include "Editor.h"

#include "EditorApplication.h"
#include "gui/imgui/ImGuiLayer.h"

#include <engine/rendering/Window.h>
#include <engine/rendering/Renderer.h>


namespace editor
{
    Editor::Editor()
    {
        working_dir_ = "C:\\Files\\Code\\C++\\rainfall_engine\\game\\";
    }

    void Editor::run(EditorApplication& app)
    {
        // Start
        engine::Window window("Rainfall Editor", 1660, 1080);
        window.set_icon("editor/assets/cdicon.jpg");

        engine::Renderer renderer(window, true);
        ImGuiLayer layer;

        app.set_window(window.get_glfw_window());
        layer.init(window.get_glfw_window());

        app.on_start();
        app.resource_manager->get_mesh_manager()->compile();

        while (app.is_running() && !window.should_close())
        {
            renderer.set_current_scene(*app.scene_manager->get_current_scene());

            // Update
            renderer.update();
            app.input_manager->set_delta_time(renderer.delta_time);
            app.on_update(renderer.delta_time);

            // Render
            layer.on_begin_frame();
            renderer.render();
            app.on_render(renderer);
            layer.on_end_frame();

            renderer.swap_and_poll();
        }

        // Shutdown
        app.on_shutdown();
        layer.shutdown();
    }
}
