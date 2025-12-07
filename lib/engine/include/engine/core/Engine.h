#pragma once

#include <engine/rendering/Window.h>
#include <engine/rendering/Renderer.h>
#include <engine/world/Scene.h>
#include <engine/src/Rendering/Entities/Entity.h>
#include <engine/src/Rendering/Buffers/Screen/GBuffer.h>

#include <type_traits>
#include "Application.h"


namespace engine
{
    template <typename A>
    void Run()
    {
        static_assert(std::is_base_of_v<Application, A>, "A must derive from Application");


        engine::Window window("Rainfall Engine", 1660, 1080);
        engine::Renderer renderer(window);

        A app;
        app.on_start();
        app.resource_manager->get_mesh_manager()->compile();

        while (app.is_running() && !window.should_close())
        {
            renderer.update();
            app.on_update(renderer.delta_time);
            renderer.render(*app.scene_manager->get_current_scene());
        }

        app.on_shutdown();
    }
}
