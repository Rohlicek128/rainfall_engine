#pragma once

#include <engine/managers/InputManager.h>
#include <engine/world/components/BehaviorComponent.h>

namespace game
{
    class CameraScript : public BehaviorComponent
    {
        engine::InputManager* input_ = nullptr;
        float speed_;

    public:
        int max_health, health;

        void on_start() override;
        void on_update(const float delta_time) override;
        void on_trigger(Entity& other, const engine::physics::CollisionsPoints& points) override;

        void set_input_manager(engine::InputManager* input);
    };
}
