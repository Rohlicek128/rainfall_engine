#include "BulletScript.h"
#include <engine/world/Entity.h>
#include <engine/world/Components/CameraComponent.h>

#include <glfw3.h>


namespace game
{
    void BulletScript::on_start()
    {
        damage = 10;
        entered = false;
        speed_ = 100.0f;
        self->transform->position += forward_ * 0.5f;
    }

    void BulletScript::on_update(const float delta_time)
    {
        self->transform->position += forward_ * (speed_ * delta_time);
    }

    void BulletScript::set_forward(glm::vec3 forward)
    {
        forward_ = forward;
    }
}
