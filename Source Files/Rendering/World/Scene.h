#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Mesh.h"
#include "../Gui/IGui.h"
#include "Loading/ISerializable.h"

class Entity;

class Scene : public IGui, ISerializable
{
    char name_edit_[64] = "Untitled";
    
    void serialize(YAML::Emitter& out) override;
    bool deserialize(YAML::Node& node) override;
public:
    std::string name;
    std::string save_path;

    std::unique_ptr<Entity> root_entity;
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<Entity*> lights;
    Mesh* mesh;
    
    std::unique_ptr<Entity> editor_camera;
    Entity* player_camera;
    Entity* current_camera;
    
    std::unique_ptr<Entity> skybox;

    
    bool opened_gui;
    Entity* selected_entity;

    Scene(const std::string&, Mesh*);
    ~Scene() override = default;
    
    void add_entity(std::unique_ptr<Entity>);
    void remove_entity(Entity*);
    void remove_entity(int);
    void add_light(Entity*);
    bool check_light(Entity*);
    Entity* find_entity_by_id(unsigned int);

    void set_scene_graph();
    void set_graph_children(const std::vector<Entity*>&, Entity*&);
    void set_entity_inspector();

    void reset();
    void save(const std::string& path);
    bool load(const std::string& path);
    
    void set_gui() override;
};
