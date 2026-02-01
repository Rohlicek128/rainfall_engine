#pragma once

#include <vector>
#include <glm/vec3.hpp>


class Entity;
class RidgidbodyComponent;

namespace engine::physics
{
    class Collider;

    struct RidgidbodyCollider
    {
        RidgidbodyComponent* ridgidbody;
        Collider* collider;
    };

    class PhysicsWorld
    {
        std::vector<Entity*> bodies_;
        glm::vec3 gravity_ = {0, -9.81f, 0};

        void resolve_collisions(float delta_time, const std::vector<RidgidbodyCollider>& ridgidbodies);
    public:
        void add_ridgidbody(Entity* entity);
        void remove_ridgidbody(Entity* entity);

        std::vector<RidgidbodyCollider> get_enabled_ridgidbodies();
        void step(float delta_time);
    };
}
