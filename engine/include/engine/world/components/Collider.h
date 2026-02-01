#pragma once

#include "../physics/CollisionPoints.h"
#include "Component.h"


class TransformComponent;

namespace engine::physics
{
    class SphereCollider;
    class PlaneCollider;

    class Collider : public Component
    {
    public:
        virtual CollisionsPoints test_collision(
            const TransformComponent* transform,
            const Collider* collider,
            const TransformComponent* collider_transform) const = 0;

        virtual CollisionsPoints test_collision(
            const TransformComponent* transform,
            const SphereCollider* sphere,
            const TransformComponent* sphere_transform) const = 0;

        virtual CollisionsPoints test_collision(
            const TransformComponent* transform,
            const PlaneCollider* plane,
            const TransformComponent* plane_transform) const = 0;
    };
}
