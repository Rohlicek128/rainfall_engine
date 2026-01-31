#include "engine/world/Entity.h"

#include "engine/world/Components/CameraComponent.h"
#include "engine/world/Components/LightComponent.h"
#include "engine/world/Components/MaterialComponent.h"
#include "engine/world/Components/TextureComponent.h"
#include "engine/world/Components/MeshComponent.h"
#include "engine/world/components/BehaviorComponent.h"

unsigned int Entity::global_id_ = 0;

Entity::Entity(const std::string& name, TransformComponent* transform)
{
    id = ++global_id_;
    this->name = name;
    is_visible = true;
    is_root = false;
    parent = nullptr;
    owner = nullptr;

    mesh_ = nullptr;

    this->transform = transform;
}

Entity::~Entity()
= default;

Entity& Entity::operator=(const Entity& other)
{
    id = other.id;
    return *this;
}

Component* Entity::contains_component_enum(const COMPONENTS_IDS& components_id)
{
    switch (components_id)
    {
        case TRANSFORM: return contains_component<TransformComponent>();
        case MESH: return contains_component<MeshComponent>();
        case CAMERA: return contains_component<CameraComponent>();
        case MATERIAL: return contains_component<MaterialComponent>();
        case TEXTURE: return contains_component<TextureComponent>();
        case LIGHT: return contains_component<LightComponent>();
    }
    return nullptr;
}

void Entity::set_mesh_to_component(Mesh& mesh)
{
    mesh_ = &mesh;
    if (MeshComponent* mesh_component = get_component<MeshComponent>())
        mesh_component->set_mesh_manager(mesh_);
}

std::vector<unsigned int> Entity::get_children_ids()
{
    return children_ids_;
}

bool Entity::add_child(Entity* child)
{
    if (child == this) return false;
    if (child->parent != nullptr) child->parent->remove_child(child);

    child->parent = this;
    children.push_back(child);
    return true;
}

bool Entity::remove_child(Entity* child)
{
    for (int i = 0; i < children.size(); ++i)
    {
        if (children.at(i) == child)
        {
            children.erase(children.begin() + i);
            child->parent = nullptr;
            return true;
        }
    }
    return false;
}

glm::mat4 Entity::get_model_matrix()
{
    if (parent != nullptr && !parent->is_root) return parent->get_model_matrix() * transform->get_model_matrix();
    return transform->get_model_matrix();
}


void Entity::serialize(YAML::Emitter& out)
{
    out << YAML::BeginMap;

    out << YAML::Key << "Entity" << YAML::Value << name.c_str();
    out << YAML::Key << "Id" << YAML::Value << id;
    out << YAML::Key << "Is Visible" << YAML::Value << is_visible;

    //Hierarchy
    if (parent) out << YAML::Key << "Parent Id" << YAML::Value << parent->id;
    out << YAML::Key << "Children Ids" << YAML::Value << YAML::Flow << YAML::BeginSeq;
    for (int i = 0; i < children.size(); ++i)
    {
        out << children.at(i)->id;
    }
    out << YAML::EndSeq;

    //Components
    out << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;
    transform->serialize(out);
    for (int i = 0; i < components.size(); ++i)
    {
        components.at(i)->serialize(out);
    }
    out << YAML::EndSeq;

    out << YAML::EndMap;
}

bool Entity::deserialize(YAML::Node& node)
{
    if (!node["Entity"]) return false;

    name = node["Entity"].as<std::string>();
    id = node["Id"].as<unsigned int>();
    is_visible = node["Is Visible"].as<bool>();
    parent = nullptr;

    if (YAML::Node children_des = node["Children Ids"])
    {
        for (auto children_id : children_des)
        {
            children_ids_.push_back(children_id.as<unsigned int>());
        }
    }

    if (YAML::Node components_des = node["Components"])
    {
        for (auto component_des : components_des)
        {
            if (component_des["Transform"])
            {
                YAML::Node comp_node = component_des["Transform"];
                transform->deserialize(comp_node);
            }
            else if (component_des["Mesh"])
            {
                YAML::Node comp_node = component_des["Mesh"];
                MeshComponent mesh(0, GL_TRIANGLES, mesh_);
                mesh.deserialize(comp_node);
                insert_component<MeshComponent>(mesh);
            }
            else if (component_des["Camera"])
            {
                YAML::Node comp_node = component_des["Camera"];
                CameraComponent camera(transform);
                camera.deserialize(comp_node);
                insert_component<CameraComponent>(camera);
            }
            else if (component_des["Material"])
            {
                YAML::Node comp_node = component_des["Material"];
                MaterialComponent material(glm::vec4(0.0f));
                material.deserialize(comp_node);
                insert_component<MaterialComponent>(material);
            }
            else if (component_des["Texture"])
            {
                YAML::Node comp_node = component_des["Texture"];
                TextureComponent texture(GL_TEXTURE_2D, 1, 1);
                texture.deserialize(comp_node);
                insert_component<TextureComponent>(texture);
            }
            else if (component_des["Light"])
            {
                YAML::Node comp_node = component_des["Light"];
                LightComponent light(lights::LIGHT_TYPE::POINT, glm::vec3(1.0f));
                light.deserialize(comp_node);
                insert_component<LightComponent>(light);
            }
            else if (component_des["Behavior"])
            {
                YAML::Node comp_node = component_des["Behavior"];
                BehaviorComponent behavior;
                behavior.deserialize(comp_node);
                insert_component<BehaviorComponent>(behavior);
            }
        }
    }

    return true;
}
