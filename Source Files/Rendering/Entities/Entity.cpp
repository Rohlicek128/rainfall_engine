#include "Entity.h"

#include <iostream>

#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Components/MaterialComponent.h"
#include "Components/TextureComponent.h"

unsigned int Entity::global_id_ = 0;

Entity::Entity(const std::string& name, TransformComponent* transform)
{
    id = ++global_id_;
    this->name = name;
    is_visible = true;
    is_root = false;

    this->transform = transform;

    components = new std::map<components_ids, Component*>();
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

bool Entity::add_component(components_ids id, Component* component)
{
    const auto result = components->insert(std::pair<components_ids, Component*>(id, component));
    return result.second;
}

bool Entity::component_exists(const components_ids id)
{
    return components->count(id) && components->at(id)->is_enabled;
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
    if (!parent->is_root && parent != nullptr) return parent->get_model_matrix() * transform->get_model_matrix();
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
        if (ImGui::Button("Rename")) name = name_edit_;
        ImGui::EndPopup();
    }
    ImGui::TextDisabled("ID: %i", id);

    ImGui::Separator();
    
    ImGui::Checkbox("Is Visible", &is_visible);

    transform->set_gui();

    bool is_opened;
    for (const auto component : *components)
    {
        ImGui::Checkbox(("##Enabled" + std::to_string(component.first)).c_str(), &component.second->is_enabled);
        ImGui::SameLine();
        
        is_opened = true;
        if (ImGui::CollapsingHeader(to_string(component.first), &is_opened, ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (!component.second->is_enabled) ImGui::BeginDisabled();
            component.second->set_gui();
            if (!component.second->is_enabled) ImGui::EndDisabled();
        }
        if (!is_opened)
        {
            components->erase(component.first);
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
            if (components->count((components_ids)i)) continue;
            if (strcmp(component_search_, "") != 0 &&
                !check_search_string(to_string((components_ids)i), component_search_, std::size(component_search_)))
                continue;
            
            if (ImGui::Selectable(to_string((components_ids)i), add_selected_ == i)) add_selected_ = i;
            if (add_selected_ == i) ImGui::SetItemDefaultFocus();
            
        }
        ImGui::EndChild();
        ImGui::EndMenu();
    }

    if (add_selected_ == -1) return;
    switch ((components_ids)add_selected_)
    {
        case TRANSFORM: add_component(TRANSFORM, new TransformComponent(glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0)));break;
        case MESH: add_component(MESH, new MeshComponent(0)); break;
        case CAMERA: add_component(CAMERA, new CameraComponent(transform)); break;
        case MATERIAL: add_component(MATERIAL, new MaterialComponent(glm::vec4(1.0))); break;
        case TEXTURE: add_component(TEXTURE, new TextureComponent(1, 1)); break;
        case LIGHT: add_component(LIGHT, new LightComponent(DIRECTIONAL, glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(0.8f))); break;
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