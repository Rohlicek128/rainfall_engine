#include "TestCollisionsAlgorithms.h"

#include "engine/world/physics/CollisionPoints.h"
#include "engine/world/components/SphereCollider.h"
#include "engine/world/components/PlaneCollider.h"
#include "engine/world/components/TransformComponent.h"


namespace engine::physics::colls
{
    // Taken from https://github.com/IainWinter/IwEngine/blob/3e2052855fea85718b7a499a7b1a3befd49d812b/IwEngine/include/iw/physics/impl/TestCollision.h
    CollisionsPoints find_sphere_sphere_collision_points(
        const SphereCollider* a, const TransformComponent* at,
        const SphereCollider* b, const TransformComponent* bt
    )
    {
        glm::vec3 a_center = a->center + at->position;
		glm::vec3 b_center = b->center + bt->position;

		float a_max_scale = glm::max(at->scale.x, glm::max(at->scale.y, at->scale.z));
        float b_max_scale = glm::max(bt->scale.x, glm::max(bt->scale.y, bt->scale.z));

		float a_radius = a->radius * a_max_scale;
		float b_radius = b->radius * b_max_scale;


		float distance = glm::distance(a_center, b_center);

		if (distance < 0.00001f || distance > a_radius + b_radius)
		    return CollisionsPoints(false);

		glm::vec3 normal = glm::normalize(b_center - a_center);

		glm::vec3 a_deep = a_center + normal * a_radius;
		glm::vec3 b_deep = b_center - normal * b_radius;

		float depth = (a_radius + b_radius) - distance;

		return CollisionsPoints(true, a_deep, b_deep, normal, depth);
    }

    CollisionsPoints find_sphere_plane_collision_points(
        const SphereCollider* a, const TransformComponent* at,
        const PlaneCollider* b, const TransformComponent* bt
    )
    {
        glm::vec3 sphere_center = at->position + a->center;
        float max_scale = glm::max(at->scale.x, glm::max(at->scale.y, at->scale.z));
        float sphere_radius = a->radius * max_scale;

        glm::vec3 plane_normal = glm::normalize(bt->rotation * b->plane);
        glm::vec3 point_on_plane = bt->position + (plane_normal * b->distance);

        float dist_from_plane = glm::dot(sphere_center - point_on_plane, plane_normal);
        float absolute_dist = glm::abs(dist_from_plane);

        if (absolute_dist > sphere_radius) return CollisionsPoints(false);

        if (dist_from_plane < 0) {
            plane_normal *= -1.0f;
        }

        glm::vec3 collision_normal = -plane_normal;

        float depth = sphere_radius - dist_from_plane;

        glm::vec3 a_deep = sphere_center - plane_normal * sphere_radius;
        glm::vec3 b_deep = sphere_center - plane_normal * dist_from_plane;

        return CollisionsPoints(true, a_deep, b_deep, collision_normal, depth);
    }

    CollisionsPoints find_plane_sphere_collision_points(
        const PlaneCollider* a, const TransformComponent* at,
        const SphereCollider* b, const TransformComponent* bt
    )
    {
        glm::vec3 world_normal = glm::normalize(at->rotation * a->plane);

        glm::vec3 sphere_center = bt->position + b->center;
        float max_scale = glm::max(bt->scale.x, glm::max(bt->scale.y, bt->scale.z));
        float sphere_radius = b->radius * max_scale;

        glm::vec3 point_on_plane = at->position + (world_normal * a->distance);


        float dist_from_plane = glm::dot(sphere_center - point_on_plane, world_normal);
        float absolute_dist = glm::abs(dist_from_plane);

        if (absolute_dist > sphere_radius) return CollisionsPoints(false);

        if (dist_from_plane < 0) {
            world_normal *= -1.0f;
        }

        glm::vec3 collision_normal = world_normal;

        float depth = sphere_radius - dist_from_plane;

        glm::vec3 b_deep = sphere_center - world_normal * sphere_radius;
        glm::vec3 a_deep = sphere_center - world_normal * dist_from_plane;

        return CollisionsPoints(true, a_deep, b_deep, collision_normal, depth);
    }
}
