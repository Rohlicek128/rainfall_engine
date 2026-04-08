#include "EnemyScript.h"
#include "BulletScript.h"
#include "engine/world/components/MaterialComponent.h"
#include "glm/ext/vector_float3.hpp"
#include <engine/world/Entity.h>

#include <glfw3.h>


namespace game
{
    void EnemyScript::on_start()
    {
        max_health = 100;
        health = 100;
        speed_ = 11.0f;
    }

    void EnemyScript::on_update(const float delta_time)
    {
        if (health <= 0) return;

        glm::vec3 direction = target->position - self->transform->position;
        float distance = glm::length(direction);

        if (distance > 0.01f)
        {
            glm::vec3 velocity = (direction / distance) * speed_ * delta_time;

            if (speed_ * delta_time >= distance)
            {
                self->transform->position = target->position;
            }
            else
            {
                self->transform->position += velocity;
            }
        }
    }

    void EnemyScript::on_trigger(Entity& other, const engine::physics::CollisionsPoints& points)
    {
        if (other.contains_component<BulletScript>())
        {
            health -= 3;
            if (health <= 0)
            {
                if (MaterialComponent* mat = self->get_component<MaterialComponent>())
                    mat->color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
                self->transform->position.y = -0.75f;
            }
        }
    }
}
