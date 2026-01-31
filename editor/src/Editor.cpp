#include "Editor.h"

#include "EditorApplication.h"
#include "gui/imgui/ImGuiLayer.h"

#include <engine/rendering/Window.h>
#include <engine/rendering/Renderer.h>
#include <engine/rendering/ui/RmlUiLayer.h>


namespace editor
{
    void run()
    {
        // Start
        EditorApplication app;

        engine::Window window("Rainfall Editor", 1660, 1080);
        window.set_icon("engine/assets/rainfall_logo_icon.png");

        engine::Renderer renderer(window, true);
        ImGuiLayer imgui_layer;
        engine::RmlUiLayer rmlui_layer(window.get_width(), window.get_height(), true);

        app.set_window(window);
        app.set_ui_context(*rmlui_layer.get_context());
        imgui_layer.init(window.get_glfw_window());

        app.on_start();
        app.resource_manager->get_mesh_manager()->compile();

        while (app.is_running() && !window.should_close())
        {
            renderer.set_current_scene(*app.scene_manager->get_current_scene());

            // Update
            //rmlui_layer.update(*app.input_manager);
            renderer.update();
            app.input_manager->update(renderer.delta_time);
            app.on_update(renderer.delta_time);

            // Render
            imgui_layer.on_begin_frame();
            renderer.render();
            app.on_render(renderer);
            imgui_layer.on_end_frame();
            //rmlui_layer.render(window.get_width(), window.get_height());

            renderer.swap_and_poll();
        }

        // Shutdown
        app.on_shutdown();
        imgui_layer.shutdown();
    }
}
