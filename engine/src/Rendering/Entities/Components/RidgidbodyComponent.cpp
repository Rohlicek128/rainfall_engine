#include "engine/world/components/RidgidbodyComponent.h"
#include <algorithm>

RidgidbodyComponent::RidgidbodyComponent(TransformComponent& transform, float mass, glm::vec3 velocity)
{
    this->transform = &transform;
    this->velocity = velocity;
    this->force = {0, 0, 0};

    this->mass = std::max(0.001f, mass);
    this->restitution = 1.0f;
    this->static_friction = 1.0f;
    this->dynamic_friction = 1.0f;

    this->is_simulated = true;
    this->is_dynamic = true;
}

std::string RidgidbodyComponent::get_name()
{
    return "Ridgidbody";
}

void RidgidbodyComponent::serialize(YAML::Emitter& out)
{
    out << YAML::BeginMap;
    out << YAML::Key << get_name() << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "Enabled" << YAML::Value << is_enabled;


    out << YAML::EndMap;
    out << YAML::EndMap;
}

bool RidgidbodyComponent::deserialize(YAML::Node& node)
{
    is_enabled = node["Enabled"].as<bool>();

    return true;
}
