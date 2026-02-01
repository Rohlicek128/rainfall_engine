#include "engine/world/components/PlaneCollider.h"

#include "engine/world/components/TransformComponent.h"
#include "TestCollisionsAlgorithms.h"
#include "engine/world/physics/CollisionPoints.h"


namespace engine::physics
{
    PlaneCollider::PlaneCollider(glm::vec3 plane, float distance)
    : plane(plane), distance(distance)
    {
    }

    CollisionsPoints PlaneCollider::test_collision(
        const TransformComponent* transform,
        const Collider* collider,
        const TransformComponent* collider_transform) const
    {
        return collider->test_collision(collider_transform, this, transform);
    }

    CollisionsPoints PlaneCollider::test_collision(
        const TransformComponent* transform,
        const SphereCollider* sphere,
        const TransformComponent* sphere_transform) const
    {
        return colls::find_plane_sphere_collision_points(this, transform, sphere, sphere_transform);
    }

    CollisionsPoints PlaneCollider::test_collision(
        const TransformComponent* transform,
        const PlaneCollider* plane,
        const TransformComponent* plane_transform) const
    {
        return CollisionsPoints(false);
    }


    std::string PlaneCollider::get_name()
    {
        return "Plane Collider";
    }

    void PlaneCollider::serialize(YAML::Emitter& out)
    {
        out << YAML::BeginMap;
        out << YAML::Key << get_name() << YAML::Value << YAML::BeginMap;
        out << YAML::Key << "Enabled" << YAML::Value << is_enabled;


        out << YAML::EndMap;
        out << YAML::EndMap;
    }

    bool PlaneCollider::deserialize(YAML::Node& node)
    {
        is_enabled = node["Enabled"].as<bool>();


        return true;
    }
}
