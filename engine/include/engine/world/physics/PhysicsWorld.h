#pragma once

#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include "Solver.h"


class Entity;
class RidgidbodyComponent;

namespace engine::physics
{
    class Collider;

    class RidgidbodyCollider;

    class PhysicsWorld
    {
        std::vector<Entity*> bodies_;
        std::vector<std::unique_ptr<Solver>> solvers_;
        glm::vec3 gravity_ = {0, -9.81f, 0};

        void resolve_collisions(float delta_time, std::vector<RidgidbodyCollider>& ridgidbodies);
    public:
        PhysicsWorld();

        void add_ridgidbody(Entity* entity);
        void remove_ridgidbody(Entity* entity);

        std::vector<RidgidbodyCollider> get_enabled_ridgidbodies();
        void step(float delta_time);
    };
}
