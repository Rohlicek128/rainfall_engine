#include "engine/world/physics/PhysicsWorld.h"

#include "Detection/Collision.h"
#include "engine/world/Entity.h"
#include "engine/world/components/RidgidbodyComponent.h"
#include "engine/world/physics/CollisionPoints.h"
#include "engine/world/components/SphereCollider.h"
#include "engine/world/components/Collider.h"

#include <glfw3.h>
//#include <iostream>


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

    std::vector<RidgidbodyCollider> PhysicsWorld::get_enabled_ridgidbodies()
    {
        std::vector<RidgidbodyCollider> results;

        for (int i = 0; i < bodies_.size(); ++i)
        {
            RidgidbodyComponent* body = bodies_.at(i)->get_enabled_component<RidgidbodyComponent>();
            Collider* collider = bodies_.at(i)->get_enabled_component<SphereCollider>();

            results.push_back(RidgidbodyCollider(body, collider));
        }

        return results;
    }


    void PhysicsWorld::step(float delta_time)
    {
        std::vector<RidgidbodyCollider> ridgidbodies = get_enabled_ridgidbodies();

        resolve_collisions(delta_time, ridgidbodies);


        for (RidgidbodyCollider body_collider : ridgidbodies)
        {
            RidgidbodyComponent* body = body_collider.ridgidbody;
            if (!body || !body->transform) continue;

            body->force += body->mass * gravity_;
            body->velocity += body->force / body->mass * delta_time;
            body->transform->position += body->velocity * delta_time;

            body->force = {0, 0, 0};

            //if (body->transform->position.y < 0.0f)
            //{
            //    body->transform->position.y = 0.0f;
            //    body->velocity = {0, 0, 0};
            //}
        }
    }

    void PhysicsWorld::resolve_collisions(float delta_time, const std::vector<RidgidbodyCollider>& ridgidbodies)
    {
        std::vector<Collision> collsisons;

        for (RidgidbodyCollider body_a : ridgidbodies)
        {
            for (RidgidbodyCollider body_b : ridgidbodies)
            {
                if (body_a.ridgidbody == body_b.ridgidbody) break;

                if (!body_a.collider || !body_b.collider) continue;

                CollisionsPoints points = body_a.collider->test_collision(
                    body_a.ridgidbody->transform,
                    body_b.collider,
                    body_b.ridgidbody->transform
                );

                if (points.has_collision)
                {
                    collsisons.emplace_back(&body_a, &body_b, points);
                    //std::cout << "Collision! " << glfwGetTime() << '\n';
                }
            }
        }

    }

}
