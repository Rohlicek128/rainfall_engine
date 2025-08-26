#pragma once
#include <map>
#include <string>
#include <vector>

#include "../Gui/IGui.h"
#include "Components/TransformComponent.h"
#include "Components/MeshComponent.h"

class Entity : public IGui
{
    int add_selected_;
    char component_search_[64] = "";
    char name_edit_[64] = "";
    
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
    std::map<components_ids, Component*>* components;

    Entity(const std::string&, TransformComponent*);
    ~Entity() override;
    Entity& operator=(const Entity&);
    bool add_component(components_ids, Component*);
    bool component_exists(components_ids);
    bool add_child(Entity*);
    bool remove_child(Entity*);
    glm::mat4 get_model_matrix();
    void set_gui() override;
};