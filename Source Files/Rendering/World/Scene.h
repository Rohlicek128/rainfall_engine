#pragma once
#include <memory>
#include <vector>

#include "Mesh.h"

class Scene : public IGui
{
public:
    std::string name;

    std::unique_ptr<Entity> root_entity;
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<Entity*> lights;
    std::unique_ptr<Mesh> mesh;

    std::unique_ptr<Entity> editor_camera;
    Entity* player_camera;

    
    bool opened_gui;
    Entity* selected_entity;

    Scene(const std::string&, Mesh&&);
    ~Scene() override = default;
    
    void add_entity(std::unique_ptr<Entity>, bool = false);
    void remove_entity(Entity*);
    void remove_entity(int);
    void add_light(Entity*);
    Entity* find_entity_by_id(unsigned int);

    void set_scene_graph();
    void set_graph_children(const std::vector<Entity*>&, Entity*&);
    void set_entity_inspector();
    
    void set_gui() override;
};
