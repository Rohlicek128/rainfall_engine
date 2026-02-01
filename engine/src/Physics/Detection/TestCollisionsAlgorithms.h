#pragma once


class TransformComponent;

namespace engine::physics
{
    class CollisionsPoints;
    class SphereCollider;

    // Taken from https://github.com/IainWinter/IwEngine/blob/3e2052855fea85718b7a499a7b1a3befd49d812b/IwEngine/include/iw/physics/impl/TestCollision.h
    namespace colls
    {
        CollisionsPoints find_sphere_sphere_collision_points(
            const SphereCollider* a, const TransformComponent* ta,
            const SphereCollider* b, const TransformComponent* ba
        );
    }
}
