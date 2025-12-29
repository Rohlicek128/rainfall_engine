#pragma once

#include <engine/rendering/Window.h>
#include <engine/rendering/Renderer.h>
#include <engine/world/Scene.h>
#include <engine/world/Entity.h>

#include <type_traits>
#include "Application.h"


namespace engine
{
    template <typename A>
    void Run()
    {
        static_assert(std::is_base_of_v<Application, A>, "A must derive from Application");


        // Start
        Window window("Rainfall Engine", 1660, 1080);
        Renderer renderer(window);

        A app;
        app.set_window(window.get_glfw_window());

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
            renderer.render();
            renderer.swap_and_poll();
        }

        // Shutdown
        app.on_shutdown();
    }
}
