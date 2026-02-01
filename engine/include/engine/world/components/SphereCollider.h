#pragma once

#include "Collider.h"


namespace engine::physics
{
    class SphereCollider : public Collider
    {
    public:
        glm::vec3 center;
        float radius;

        SphereCollider(glm::vec3 center = {0, 0, 0}, float radius = 1.0f);
        ~SphereCollider() override = default;

        CollisionsPoints test_collision(
            const TransformComponent* transform,
            const Collider* collider,
            const TransformComponent* collider_transform) const override;

        CollisionsPoints test_collision(
            const TransformComponent* transform,
            const SphereCollider* sphere,
            const TransformComponent* sphere_transform) const override;


        std::string get_name() override;
        void serialize(YAML::Emitter& out) override;
        bool deserialize(YAML::Node& node) override;
    };
}
