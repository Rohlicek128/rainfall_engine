#include "engine/world/components/SphereCollider.h"

#include "engine/world/components/TransformComponent.h"
#include "TestCollisionsAlgorithms.h"


namespace engine::physics
{
    SphereCollider::SphereCollider(glm::vec3 center, float radius)
    : center(center), radius(radius)
    {
    }

    CollisionsPoints SphereCollider::test_collision(
        const TransformComponent* transform,
        const Collider* collider,
        const TransformComponent* collider_transform) const
    {
        return collider->test_collision(collider_transform, this, transform);
    }

    CollisionsPoints SphereCollider::test_collision(
        const TransformComponent* transform,
        const SphereCollider* sphere,
        const TransformComponent* sphere_transform) const
    {
        return colls::find_sphere_sphere_collision_points(this, transform, sphere, sphere_transform);
    }


    std::string SphereCollider::get_name()
    {
        return "Sphere Collider";
    }

    void SphereCollider::serialize(YAML::Emitter& out)
    {
        out << YAML::BeginMap;
        out << YAML::Key << get_name() << YAML::Value << YAML::BeginMap;
        out << YAML::Key << "Enabled" << YAML::Value << is_enabled;


        out << YAML::EndMap;
        out << YAML::EndMap;
    }

    bool SphereCollider::deserialize(YAML::Node& node)
    {
        is_enabled = node["Enabled"].as<bool>();


        return true;
    }
}
