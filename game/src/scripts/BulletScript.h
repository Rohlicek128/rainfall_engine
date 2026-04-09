#pragma once
#include <engine/world/components/BehaviorComponent.h>

namespace game
{
    class BulletScript : public BehaviorComponent
    {
        float speed_;
        glm::vec3 forward_;

    public:
        int damage;
        bool entered;

        void on_start() override;
        void on_update(const float delta_time) override;

        void set_forward(glm::vec3 forward);
    };
}
