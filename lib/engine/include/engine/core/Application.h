#pragma once

#include "engine/managers/ResourceManager.h"
#include "engine/managers/SceneManager.h"
#include "engine/world/Entity.h"
#include <memory>


namespace engine
{
    class Application
    {
        bool is_running_ = true;

    public:
        std::unique_ptr<ResourceManager> resource_manager;
        std::unique_ptr<SceneManager> scene_manager;

        Application();

        bool is_running();
        void stop();

        virtual void on_start() {}
        virtual void on_update(const float delta_time) {}
        virtual void on_shutdown() {}
    };
}
