#include "engine/world/components/BehaviorComponent.h"

#include <iostream>


BehaviorComponent::BehaviorComponent()
{
    name = "N/A";
    active = false;
    self = nullptr;
}

void BehaviorComponent::set_owner_entity(Entity& owner)
{
    self = &owner;
}

bool BehaviorComponent::is_active()
{
    return active;
}

void BehaviorComponent::start()
{
    on_start();
    active = true;
}


std::string BehaviorComponent::get_name()
{
    return "Behavior";
}

void BehaviorComponent::serialize(YAML::Emitter& out)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Behavior" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "Enabled" << YAML::Value << is_enabled;

    out << YAML::Key << "Name" << YAML::Value << get_name();

    out << YAML::EndMap;
    out << YAML::EndMap;
}

bool BehaviorComponent::deserialize(YAML::Node& node)
{
    is_enabled = node["Enabled"].as<bool>();

    name = node["Name"].as<std::string>();

    return true;
}
