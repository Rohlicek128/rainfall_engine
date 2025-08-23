#pragma once
#include <map>
#include <string>

#include "../Gui/IGui.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"

class Entity : public IGui
{
    int add_selected_;
    char component_search_[64] = "";
    //char name_edit_[64] = "";
    
    bool check_search_string(const char*, const char*, int);
public:
    std::string name;
    bool is_visible;
    
    TransformComponent* transform;
    MeshComponent* mesh;
    std::map<components_ids, Component*>* components;

    Entity(const std::string&, TransformComponent*, MeshComponent* = nullptr);
    ~Entity() override = default;
    bool add_component(components_ids, Component*);
    bool component_exists(components_ids);
    void set_gui() override;
};