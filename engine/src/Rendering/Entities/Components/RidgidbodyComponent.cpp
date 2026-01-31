#include "engine/world/components/RidgidbodyComponent.h"
#include <algorithm>

RidgidbodyComponent::RidgidbodyComponent(TransformComponent& transform, glm::vec3 velocity, glm::vec3 force, float mass)
{
    this->transform = &transform;
    this->velocity = velocity;
    this->force = force;
    this->mass = std::max(0.001f, mass);
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
