#include "engine/world/components/BehaviorComponent.h"


BehaviorComponent::BehaviorComponent()
{
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

}

bool BehaviorComponent::deserialize(YAML::Node& node)
{
    return true;
}
