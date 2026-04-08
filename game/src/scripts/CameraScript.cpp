#include "CameraScript.h"
#include "EnemyScript.h"
#include <engine/world/Entity.h>
#include <engine/world/Components/CameraComponent.h>

#include <glfw3.h>


namespace game
{
    void CameraScript::on_start()
    {
        speed_ = 10.0f;
        max_health = 100;
        health = 100;
    }

    void CameraScript::on_update(const float delta_time)
    {
        glm::vec3 forward = self->transform->rotation;
        glm::vec3 flatForward = glm::normalize(glm::vec3(forward.x, 0.0f, forward.z));
        glm::vec3 flatRight = glm::normalize(glm::cross(flatForward, glm::vec3(0.0f, 1.0f, 0.0f)));

        if (input_->get_key_down(GLFW_KEY_W))
            self->transform->position += flatForward * speed_ * delta_time;
        if (input_->get_key_down(GLFW_KEY_S))
            self->transform->position -= flatForward * speed_ * delta_time;
        if (input_->get_key_down(GLFW_KEY_A))
            self->transform->position -= flatRight * speed_ * delta_time;
        if (input_->get_key_down(GLFW_KEY_D))
            self->transform->position += flatRight * speed_ * delta_time;

        if (CameraComponent* cur_camera = self->get_component<CameraComponent>())
            cur_camera->default_mouse_move(*input_, delta_time);
    }

    void CameraScript::on_trigger(Entity& other, const engine::physics::CollisionsPoints& points)
    {
        if (other.contains_component<EnemyScript>())
        {
            health -= 1;
            if (health <= 0)
            {
            }
        }
    }

    void CameraScript::set_input_manager(engine::InputManager* input)
    {
        input_ = input;
    }

}
