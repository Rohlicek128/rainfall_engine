#pragma once
#include "engine/managers/SceneManager.h"
#include "engine/world/components/TransformComponent.h"
#include <engine/world/components/BehaviorComponent.h>

namespace game
{
    class EnemyScript : public BehaviorComponent
    {
        int max_health, health;
        float speed_;

    public:
        engine::SceneManager* scene_manager;
        TransformComponent* target;

        void on_start() override;
        void on_update(const float delta_time) override;
        void on_trigger(Entity& other, const engine::physics::CollisionsPoints& points) override;
    };
}
