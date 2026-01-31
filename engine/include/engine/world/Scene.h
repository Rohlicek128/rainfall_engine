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
class MeshComponent;
class LightComponent;
class BehaviorComponent;


class Scene : ISerializable
{
    char name_edit_[64] = "Untitled";

    void set_mesh_to_entity(Entity* entity);

    void serialize(YAML::Emitter& out) override;
    bool deserialize(YAML::Node& node) override;
public:
    std::string name;
    std::string save_path;

    std::unique_ptr<Entity> root_entity;
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<Entity*> lights;
    std::vector<Entity*> behaviors;
    Mesh* mesh;

    std::unique_ptr<Entity> editor_camera;
    Entity* player_camera;
    Entity* current_camera;

    std::unique_ptr<Entity> skybox;

    Scene(const std::string&);
    ~Scene() override = default;

    void set_mesh(Mesh& mesh);

    void add_entity(std::unique_ptr<Entity>);
    Entity* create_entity(const std::string& name = "Untitled");
    void remove_entity(Entity*);
    void remove_entity(int);

    template<typename C>
    void on_add_component(Entity& entity)
    {
        if constexpr (std::is_same_v<C, MeshComponent>)
            set_mesh_to_entity(&entity);
        if constexpr (std::is_same_v<C, LightComponent>)
            lights.push_back(&entity);
        if constexpr (std::is_base_of_v<BehaviorComponent, C>)
            behaviors.push_back(&entity);
    }

    bool check_light(Entity*);
    Entity* find_entity_by_id(unsigned int);
    std::vector<Entity*> get_lights_by_type(lights::LIGHT_TYPE type);

    void set_player_camera(Entity& camera);

    void reset();
    void save(const std::string& path);
    bool load(const std::string& path);
};
