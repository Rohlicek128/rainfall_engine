#pragma once

#include "Component.h"


class Entity;

class BehaviorComponent : public Component
{
    bool active;

protected:
    Entity* self;

public:
    std::string name;

    BehaviorComponent();
    ~BehaviorComponent() override = default;

    void set_owner_entity(Entity& owner);
    bool is_active();

    void start();
    virtual void on_start() {}
    virtual void on_update(const float delta_time) {}
    virtual void on_shutdown() {}

    virtual std::string get_name() override;
    void serialize(YAML::Emitter& out) override;
    bool deserialize(YAML::Node& node) override;
};
