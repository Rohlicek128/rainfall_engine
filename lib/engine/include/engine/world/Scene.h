#pragma once
#include <memory>
#include <string>
#include <vector>


#include "../core/ISerializable.h"

namespace lights
{
    enum class LIGHT_TYPE : int;
}
class Entity;
class Mesh;


class Scene : ISerializable
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

    Scene(const std::string&);
    ~Scene() override = default;

    void set_mesh(Mesh& mesh);

    void add_entity(std::unique_ptr<Entity>);
    Entity* create_entity(const std::string& name = "Unnamed");
    void remove_entity(Entity*);
    void remove_entity(int);
    void add_light(Entity*);
    bool check_light(Entity*);
    Entity* find_entity_by_id(unsigned int);
    std::vector<Entity*> get_lights_by_type(lights::LIGHT_TYPE type);

    void set_scene_graph();
    void set_graph_children(const std::vector<Entity*>&, Entity*&);
    void set_entity_inspector();

    void reset();
    void save(const std::string& path);
    bool load(const std::string& path);
};
