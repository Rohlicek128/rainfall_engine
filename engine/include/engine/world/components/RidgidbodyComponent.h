#pragma once
#include "Component.h"
#include "TransformComponent.h"


class RidgidbodyComponent : public Component
{


public:
    TransformComponent* transform;
    glm::vec3 velocity;
    glm::vec3 force;
    float mass;
    float restitution;
    float static_friction, dynamic_friction;

    bool is_simulated, is_dynamic;

    RidgidbodyComponent(TransformComponent& transform, float mass = 1.0f, glm::vec3 velocity = {0.0f, 0.0f, 0.0f});
    ~RidgidbodyComponent() override = default;

    std::string get_name() override;
    void serialize(YAML::Emitter& out) override;
    bool deserialize(YAML::Node& node) override;
};
