#pragma once
#include <type_traits>
#include <string>
#include <vector>

#include "engine/core/ISerializable.h"
#include "components/TransformComponent.h"
#include "engine/world/components/BehaviorComponent.h"

class Mesh;

class Entity : public ISerializable
{
    int add_selected_;
    char component_search_[64] = "";
    char name_edit_[64] = "";

    std::vector<unsigned int> children_ids_;
    Mesh* mesh_;

    bool check_search_string(const char*, const char*, int);
protected:
    static unsigned int global_id_;
public:
    unsigned int id;
    std::string name;
    bool is_visible;

    bool is_root;
    Entity* parent;
    std::vector<Entity*> children;

    TransformComponent* transform;
    std::vector<std::unique_ptr<Component>> components;

    Entity(const std::string&, TransformComponent*);
    ~Entity() override;
    Entity& operator=(const Entity&);

    template<typename C, typename... Args>
    void add_component(Args&&... args);
    template<typename C>
    void insert_component(C&);
    template<typename C>
    C* get_component();

    template<typename C>
    C* contains_component();
    Component* contains_component_enum(const COMPONENTS_IDS&);
    template<typename C>
    C* get_enabled_component();

    void set_mesh_to_component(Mesh*);
    std::vector<unsigned int> get_children_ids();

    bool add_child(Entity*);
    bool remove_child(Entity*);
    glm::mat4 get_model_matrix();

    void serialize(YAML::Emitter& out) override;
    bool deserialize(YAML::Node& node) override;
};

template<typename C, typename... Args>
void Entity::add_component(Args&&... args)
{
    static_assert(std::is_base_of_v<Component, C>, "C must derive from Component");
    components.push_back(std::make_unique<C>(std::forward<Args>(args)...));

    if (std::is_base_of_v<BehaviorComponent, C>)
    {
        get_component<BehaviorComponent>()->set_owner_entity(*this);
    }
}

template<typename C>
void Entity::insert_component(C& component)
{
    static_assert(std::is_base_of_v<Component, C>, "C must derive from Component");
    std::unique_ptr<C> comp = std::make_unique<C>(component);
    components.push_back(std::move(comp));
}

template<typename C>
C* Entity::get_component()
{
    static_assert(std::is_base_of_v<Component, C>, "C must derive from Component");
    for (int i = 0; i < components.size(); ++i)
    {
        if (C* component = dynamic_cast<C*>(components.at(i).get()))
            return component;
    }
    return nullptr;
}

template<typename C>
C* Entity::contains_component()
{
    return get_component<C>();
}

template<typename C>
C* Entity::get_enabled_component()
{
    static_assert(std::is_base_of_v<Component, C>, "C must derive from Component");
    for (int i = 0; i < components.size(); ++i)
    {
        if (C* component = dynamic_cast<C*>(components.at(i).get()))
        {
            if (components.at(i)->is_enabled)
                return component;
        }
    }
    return nullptr;
}
