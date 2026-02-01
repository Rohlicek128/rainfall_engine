#pragma once

#include "Collider.h"


namespace engine::physics
{
    class PlaneCollider : public Collider
    {
    public:
        glm::vec3 plane;
        float distance;

        PlaneCollider(glm::vec3 plane = {0, 0, 0}, float distance = 1.0f);
        ~PlaneCollider() override = default;

        CollisionsPoints test_collision(
            const TransformComponent* transform,
            const Collider* collider,
            const TransformComponent* collider_transform) const override;

        CollisionsPoints test_collision(
            const TransformComponent* transform,
            const SphereCollider* sphere,
            const TransformComponent* sphere_transform) const override;

        CollisionsPoints test_collision(
            const TransformComponent* transform,
            const PlaneCollider* plane,
            const TransformComponent* plane_transform) const override;


        std::string get_name() override;
        void serialize(YAML::Emitter& out) override;
        bool deserialize(YAML::Node& node) override;
    };
}
