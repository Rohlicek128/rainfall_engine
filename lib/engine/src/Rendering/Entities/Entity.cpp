#include "engine/world/Entity.h"

#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Components/MaterialComponent.h"
#include "Components/TextureComponent.h"
#include "Components/MeshComponent.h"

unsigned int Entity::global_id_ = 0;

Entity::Entity(const std::string& name, TransformComponent* transform)
{
    id = ++global_id_;
    this->name = name;
    is_visible = true;
    is_root = false;
    parent = nullptr;

    mesh_ = nullptr;

    this->transform = transform;

    add_selected_ = -1;
    name.copy(name_edit_, std::size(name_edit_));
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

void Entity::set_mesh_to_component(Mesh* mesh)
{
    mesh_ = mesh;
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

void Entity::set_gui()
{
    ImGui::PushFont(nullptr, ImGui::GetStyle().FontSizeBase * 2.0f);
    ImGui::Selectable(name.c_str());
    ImGui::PopFont();
    if (ImGui::BeginPopupContextItem())
    {
        ImGui::InputText("##InputName", name_edit_, std::size(name_edit_));
        ImGui::SetNextItemShortcut(ImGuiKey_Enter);
        if (ImGui::Button("Rename"))
        {
            name = name_edit_;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    ImGui::SetItemTooltip("Right-click to Rename");

    ImGui::TextDisabled("ID: %i", id);

    ImGui::Separator();

    ImGui::Checkbox("Is Visible", &is_visible);

    transform->set_gui();

    bool is_opened;
    for (int i = 0; i < components.size(); ++i)
    {
        ImGui::Checkbox(("##EnabledEC" + std::to_string(i + 1)).c_str(), &components.at(i)->is_enabled);
        ImGui::SameLine();

        is_opened = true;
        if (ImGui::CollapsingHeader(components.at(i)->get_name().c_str(), &is_opened, ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (!components.at(i)->is_enabled) ImGui::BeginDisabled();
            components.at(i)->set_gui();
            if (!components.at(i)->is_enabled) ImGui::EndDisabled();
        }
        if (!is_opened)
        {
            components.erase(components.begin() + i);
            break;
        }
    }

    //Add Component
    ImGui::Separator();
    if (ImGui::BeginMenu("Add Component"))
    {
        ImGui::SeparatorText("Select a Component:");
        ImGui::InputTextWithHint("##Search", "Search", component_search_, std::size(component_search_));

        ImGui::BeginChild("##SelectComponent", ImVec2(-FLT_MIN, 7 * ImGui::GetTextLineHeightWithSpacing()), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY);
        for (int i = 1; i < 6; ++i)
        {
            if (contains_component_enum((COMPONENTS_IDS)i)) continue;
            if (strcmp(component_search_, "") != 0 &&
                !check_search_string(to_string((COMPONENTS_IDS)i), component_search_, std::size(component_search_)))
                continue;

            if (ImGui::Selectable(to_string((COMPONENTS_IDS)i), add_selected_ == i)) add_selected_ = i;
            if (add_selected_ == i) ImGui::SetItemDefaultFocus();

        }
        ImGui::EndChild();
        ImGui::EndMenu();
    }

    if (add_selected_ == -1) return;
    switch ((COMPONENTS_IDS)add_selected_)
    {
        case TRANSFORM: add_component<TransformComponent>(glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0)); break;
        case MESH: add_component<MeshComponent>(0, GL_TRIANGLES, mesh_); break;
        case CAMERA: add_component<CameraComponent>(transform); break;
        case MATERIAL: add_component<MaterialComponent>(glm::vec4(1.0)); break;
        case TEXTURE: add_component<TextureComponent>(GL_TEXTURE_2D); break;
        case LIGHT: add_component<LightComponent>(lights::LIGHT_TYPE::POINT, glm::vec3(1.0f)); break;
    }
    add_selected_ = -1;
}

bool Entity::check_search_string(const char* whole, const char* part, const int part_lenght)
{
    for (int i = 0; i < part_lenght; ++i)
    {
        if (part[i] == '\0') break;
        if (tolower(part[i]) != tolower(whole[i]) ) return false;
    }
    return true;
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
        }
    }

    return true;
}
