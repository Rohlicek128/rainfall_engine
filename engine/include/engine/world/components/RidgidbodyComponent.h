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

    RidgidbodyComponent(TransformComponent& transform, glm::vec3 velocity = {0.0f, 0.0f, 0.0f}, glm::vec3 force = {0.0f, 0.0f, 0.0f}, float mass = 1.0f);
    ~RidgidbodyComponent() override = default;

    std::string get_name() override;
    void serialize(YAML::Emitter& out) override;
    bool deserialize(YAML::Node& node) override;
};
