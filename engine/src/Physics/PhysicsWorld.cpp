#include "engine/world/PhysicsWorld.h"
#include "engine/world/Entity.h"
#include "engine/world/components/RidgidbodyComponent.h"


namespace engine::physics
{
    void PhysicsWorld::add_ridgidbody(Entity* entity)
    {
        if (!entity) return;
        bodies_.push_back(entity);
    }

    void PhysicsWorld::remove_ridgidbody(Entity* entity)
    {
        if (!entity) return;

        auto itr = std::find(bodies_.begin(), bodies_.end(), entity);
        if (itr == bodies_.end()) return;
        bodies_.erase(itr);
    }


    void PhysicsWorld::step(float delta_time)
    {
        for (int i = 0; i < bodies_.size(); ++i)
        {
            if (RidgidbodyComponent* body = bodies_.at(i)->get_enabled_component<RidgidbodyComponent>())
            {
                if (!body->transform) continue;

                body->force += body->mass * gravity_;
                body->velocity += body->force / body->mass * delta_time;
                body->transform->position += body->velocity * delta_time;

                body->force = {0, 0, 0};

                if (body->transform->position.y < 0.0f)
                {
                    body->transform->position.y = 0.0f;
                    body->velocity = {0, 0, 0};
                }
            }
        }
    }
}
