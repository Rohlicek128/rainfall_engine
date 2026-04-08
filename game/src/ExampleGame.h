#pragma once

#include <engine/core/Application.h>
#include <queue>
#include "RmlUi/Core/ElementDocument.h"


namespace game
{
    class ExampleGame : public engine::Application
    {
        bool show_;
        Rml::String text_;
        Rml::ElementDocument* document_;

        float firerate_, firerate_count_;
        int sphere_model_;
        std::queue<Entity*> bullets_;

        float spawnrate_, spawnrate_count_;

        void move_camera(Entity& camera, const float detla_time);

    public:
        void on_start() override;
        void on_update(const float delta_time) override;
    };
}
