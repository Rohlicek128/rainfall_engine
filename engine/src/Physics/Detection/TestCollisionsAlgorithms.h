#pragma once


class TransformComponent;

namespace engine::physics
{
    class CollisionsPoints;
    class SphereCollider;
    class PlaneCollider;

    // Taken from https://github.com/IainWinter/IwEngine/blob/3e2052855fea85718b7a499a7b1a3befd49d812b/IwEngine/include/iw/physics/impl/TestCollision.h
    namespace colls
    {
        CollisionsPoints find_sphere_sphere_collision_points(
            const SphereCollider* a, const TransformComponent* at,
            const SphereCollider* b, const TransformComponent* bt
        );

        CollisionsPoints find_sphere_plane_collision_points(
            const SphereCollider* a, const TransformComponent* at,
            const PlaneCollider* b, const TransformComponent* bt
        );

        CollisionsPoints find_plane_sphere_collision_points(
            const PlaneCollider* a, const TransformComponent* at,
            const SphereCollider* b, const TransformComponent* bt
        );
    }
}
