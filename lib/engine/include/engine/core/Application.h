#pragma once

#include "engine/world/Project.h"
#include "engine/managers/ResourceManager.h"
#include "engine/managers/SceneManager.h"
#include "engine/managers/InputManager.h"
#include "engine/world/Entity.h"
#include <memory>


namespace engine
{
    class Application
    {
        bool is_running_ = true;

    public:
        std::unique_ptr<Project> current_project;

        std::unique_ptr<ResourceManager> resource_manager;
        std::unique_ptr<SceneManager> scene_manager;
        std::unique_ptr<InputManager> input_manager;

        Application();
        void set_window(GLFWwindow* window);

        void reset();

        bool is_running();
        void stop();

        float get_uptime();

        virtual void on_start() {}
        virtual void on_update(const float delta_time) {}
        virtual void on_shutdown() {}
    };
}
