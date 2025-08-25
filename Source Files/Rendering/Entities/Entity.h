#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

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

    std::shared_ptr<Entity> parent;
    std::vector<std::shared_ptr<Entity>> children;
    
    TransformComponent* transform;
    MeshComponent* mesh;
    std::map<components_ids, Component*>* components;

    Entity(const std::string&, TransformComponent*, MeshComponent* = nullptr);
    ~Entity() override = default;
    bool add_component(components_ids, Component*);
    bool component_exists(components_ids);
    void add_child(const std::shared_ptr<Entity>&);
    glm::mat4 get_model_matrix();
    void set_gui() override;
};