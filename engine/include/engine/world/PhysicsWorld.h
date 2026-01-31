#pragma once

#include <vector>
#include <glm/vec3.hpp>


class Entity;

namespace engine::physics
{
    class PhysicsWorld
    {
        std::vector<Entity*> bodies_;
        glm::vec3 gravity_ = {0, -9.81f, 0};

    public:
        void add_ridgidbody(Entity* entity);
        void remove_ridgidbody(Entity* entity);

        void step(float delta_time);
    };
}
