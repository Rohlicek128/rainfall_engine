#include "engine/world/Scene.h"

#include "engine/managers/Mesh.h"

#include <fstream>
#include <iostream>
#include <utility>

#include "engine/world/Entity.h"
#include "engine/managers/Mesh.h"

#include "engine/world/Components/CameraComponent.h"
#include "engine/world/Components/LightComponent.h"
#include "engine/world/Components/TextureComponent.h"
#include "engine/world/Components/MeshComponent.h"
#include "engine/world/components/BehaviorComponent.h"

Scene::Scene(const std::string& name)
{
    this->name = name;
    this->save_path = "N/A";

    root_entity = std::make_unique<Entity>("__root", new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f) ,glm::vec3(0.0f)));
    root_entity->is_root = true;

    editor_camera = std::make_unique<Entity>("__editor_camera",
        new TransformComponent(glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f))
        );
    editor_camera->add_component<CameraComponent>(editor_camera->transform, glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
    player_camera = nullptr;
    current_camera = editor_camera.get();

    skybox = std::make_unique<Entity>("__skybox",
        new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f))
        );
    //skybox->add_component<MeshComponent>(0, GL_TRIANGLES, this->mesh);
    skybox->add_component<TextureComponent>(0x8513, 1, 1);
}

void Scene::set_mesh(Mesh& mesh)
{
    this->mesh = &mesh;

    for (int i = 0; i < entities.size(); ++i)
    {
        entities.at(i)->set_mesh_to_component(*this->mesh);
    }
}

void Scene::add_entity(std::unique_ptr<Entity> entity)
{
    if (entity->parent == nullptr) root_entity->add_child(entity.get());
    entities.push_back(std::move(entity));
    entities.back()->owner = this;

    entities.back()->set_mesh_to_component(*mesh);

    if (entities.back()->contains_component<LightComponent>())
        lights.push_back(entities.back().get());
    if (entities.back()->contains_component<BehaviorComponent>())
        behaviors.push_back(entities.back().get());
}

Entity* Scene::create_entity(const std::string& name)
{
    std::unique_ptr<Entity> entity = std::make_unique<Entity>(name,
        new TransformComponent(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f))
    );

    add_entity(std::move(entity));
    return entities.back().get();
}

void Scene::remove_entity(Entity* entity)
{
    for (int i = 0; i < lights.size(); ++i)
    {
        if (entity == lights.at(i))
        {
            lights.erase(lights.begin() + i);
            break;
        }
    }

    for (int i = 0; i < entities.size(); ++i)
    {
        if (entity == entities.at(i).get())
        {
            entity->parent->remove_child(entity);
            for (Entity* child : entities.at(i)->children)
                child->parent = root_entity.get();

            entities.erase(entities.begin() + i);
            break;
        }
    }
}

void Scene::remove_entity(const int index)
{
    if (index < 0 || index >= entities.size()) return;
    for (Entity* child : entities.at(index)->children)
        child->parent = root_entity.get();

    entities.erase(entities.begin() + index);
    entities.resize(entities.size() - 1);
}

bool Scene::check_light(Entity* light)
{
    if (!light->contains_component<LightComponent>()) return false;

    for (int i = 0; i < lights.size(); ++i)
    {
        if (!lights.at(i)->contains_component<LightComponent>())
        {
            lights.erase(lights.begin() + i);
            return false;
        }

        if (lights.at(i)->id == light->id)
            return false;
    }

    lights.push_back(light);
    return true;
}

Entity* Scene::find_entity_by_id(const unsigned int id)
{
    for (int i = 0; i < entities.size(); ++i)
    {
        if (entities.at(i)->id == id) return entities.at(i).get();
    }
    return nullptr;
}

std::vector<Entity*> Scene::get_lights_by_type(const lights::LIGHT_TYPE type)
{
    std::vector<Entity*> result;
    for (Entity* light : lights)
    {
        if (const LightComponent* light_comp = light->get_component<LightComponent>(); light_comp->type == type)
            result.push_back(light);
    }
    return result;
}

void Scene::set_player_camera(Entity& camera)
{
    if (!camera.contains_component<CameraComponent>()) return;
    player_camera = &camera;
    current_camera = player_camera;
}

void Scene::set_mesh_to_entity(Entity* entity)
{
    entity->set_mesh_to_component(*mesh);
}


void Scene::reset()
{
    entities.clear();
    root_entity->children.clear();
    lights.clear();

    player_camera = nullptr;
    current_camera = editor_camera.get();

    name = "Untitled";
    strcpy_s(name_edit_, name.c_str());
}

void Scene::save(const std::string& path)
{
    YAML::Emitter out;

    serialize(out);

    std::ofstream file_out(path.c_str());
    file_out << out.c_str();

    save_path = path;
    std::cout << "[ENGINE] Scene: SAVED from " << path << '\n';
}

bool Scene::load(const std::string& path)
{
    const std::ifstream stream(path);
    std::stringstream str_stream;
    str_stream << stream.rdbuf();

    YAML::Node scene = YAML::Load(str_stream.str());

    reset();
    const bool result = deserialize(scene);

    save_path = path;
    std::cout << "[ENGINE] Scene: LOADED from " << path << '\n';

    return result;
}

void Scene::serialize(YAML::Emitter& out)
{
    out << YAML::BeginMap;

    out << YAML::Key << "Scene" << YAML::Value << name.c_str();
    //if (selected_entity) out << YAML::Key << "Selected Entity Id" << YAML::Value << selected_entity->id;

    out << YAML::Key << "Current Camera Id" << YAML::Value << current_camera->id;
    if (player_camera) out << YAML::Key << "Player Camera Id" << YAML::Value << player_camera->id;
    out << YAML::Key << "Editor Camera" << YAML::Value;
    editor_camera->serialize(out);

    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
    for (int i = 0; i < entities.size(); ++i)
    {
        entities.at(i)->serialize(out);
    }
    out << YAML::EndSeq;

    out << YAML::EndMap;
}

bool Scene::deserialize(YAML::Node& node)
{
    if (!node["Scene"]) return false;

    name = node["Scene"].as<std::string>();
    strcpy_s(name_edit_, name.c_str());

    int selected_entity_id = -1;
    if (YAML::Node cur_node = node["Selected Entity Id"])
        selected_entity_id = cur_node.as<unsigned int>();

    unsigned int current_cam_id = node["Current Camera Id"].as<unsigned int>();
    int player_cam_id = -1;
    if (YAML::Node cam_node = node["Player Camera Id"])
        player_cam_id = cam_node.as<unsigned int>();
    if (YAML::Node cam_node = node["Editor Camera"])
    {
        std::unique_ptr<Entity> cam = std::make_unique<Entity>("__editor_camera", new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.5f)));
        if (cam->deserialize(cam_node))
        {
            editor_camera.swap(cam);
            current_camera = editor_camera.get();
        }
    }

    if (YAML::Node entities_des = node["Entities"])
    {
        for (auto entity_des : entities_des)
        {
            std::unique_ptr<Entity> entity = std::make_unique<Entity>("", new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f) ,glm::vec3(0.0f)));
            if (entity->deserialize(entity_des))
            {
                add_entity(std::move(entity));
            }
        }
    }

    //Link Entities
    for (unsigned int i = 0; i < entities.size(); ++i)
    {
        //if (std::cmp_equal(entities.at(i)->id, selected_entity_id))
        //    selected_entity = entities.at(i).get();

        if (std::cmp_equal(entities.at(i)->id, player_cam_id))
            player_camera = entities.at(i).get();
        if (entities.at(i)->id == current_cam_id)
            current_camera = entities.at(i).get();

        for (unsigned int j = 0; j < entities.at(i)->get_children_ids().size(); ++j)
        {
            for (unsigned int k = 0; k < entities.size(); ++k)
            {
                if (entities.at(k)->id == entities.at(i)->get_children_ids().at(j))
                {
                    entities.at(i)->add_child(entities.at(k).get());
                    break;
                }
            }
        }
    }

    return true;
}
