#include "Scene.h"

#include <fstream>
#include <iostream>
#include <utility>

#include "../Entities/Entity.h"
#include "../World/Mesh.h"

#include "../Entities/Components/CameraComponent.h"
#include "../Entities/Components/LightComponent.h"
#include "../Entities/Components/TextureComponent.h"
#include "../Entities/Components/MeshComponent.h"

Scene::Scene(const std::string& name, Mesh* mesh)
{
    this->name = name;
    this->save_path = "N/A";
    this->mesh = mesh;
    
    root_entity = std::make_unique<Entity>("__root", new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f) ,glm::vec3(0.0f)));
    root_entity->is_root = true;

    editor_camera = std::make_unique<Entity>("__editor_camera",
        new TransformComponent(glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f))
        );
    editor_camera->add_component<CameraComponent>(editor_camera->transform, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
    player_camera = nullptr;
    current_camera = editor_camera.get();

    skybox = std::make_unique<Entity>("__skybox",
        new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f))
        );
    skybox->add_component<MeshComponent>(0, GL_TRIANGLES, this->mesh);
    skybox->add_component<TextureComponent>(0x8513, 1, 1);

    opened_gui = false;
    selected_entity = nullptr;
}

void Scene::add_entity(std::unique_ptr<Entity> entity)
{
    if (entity->parent == nullptr) root_entity->add_child(entity.get());
    entities.push_back(std::move(entity));
    entities.back()->set_mesh_to_component(mesh);
    
    if (entities.back()->contains_component<LightComponent>()) add_light(entities.back().get());
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

void Scene::add_light(Entity* light)
{
    lights.push_back(light);
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

    add_light(light);
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

std::vector<Entity*> Scene::get_lights_by_type(const LIGHT_TYPE type)
{
    std::vector<Entity*> result;
    for (Entity* light : lights)
    {
        if (const LightComponent* light_comp = light->get_component<LightComponent>(); light_comp->type == type)
            result.push_back(light);
    }
    return result;
}

void Scene::set_scene_graph()
{
    if (ImGui::BeginChild("##Entities", ImVec2(0, 0), ImGuiChildFlags_Borders))
    {
        //Scene
        ImGui::PushFont(nullptr, ImGui::GetStyle().FontSizeBase * 2.0f);
        ImGui::SeparatorText(name.c_str());
        ImGui::PopFont();
        if (ImGui::Button("+", {33, 33}))
        {
            std::unique_ptr<Entity> empty = std::make_unique<Entity>("Empty", new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
            add_entity(std::move(empty));
        }
        ImGui::SetItemTooltip("Add Empty Entity");
        ImGui::SameLine();
        if (ImGui::Button("-", {33, 33}) && selected_entity != nullptr)
        {
            remove_entity(selected_entity);
            selected_entity = entities.back().get();
        }
        ImGui::SetItemTooltip("Remove Entity");
        ImGui::SameLine();
        //Root
        ImGui::Button(" R ");
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_DND"))
            {
                IM_ASSERT(payload->DataSize == sizeof(Entity*));
                Entity* payload_entity = *(Entity**)payload->Data;
                if (payload_entity != nullptr && payload_entity->parent != nullptr)
                {
                    payload_entity->parent->remove_child(payload_entity);
                    root_entity->add_child(payload_entity);
                }
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::SameLine();
        if (ImGui::Button(" S "))
            opened_gui = true;
        ImGui::SameLine();
        ImGui::TextDisabled("Count: %i", entities.size());

        //Graph
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
        if (ImGui::BeginChild("##EntitiesInner"))
        {
            set_graph_children(root_entity->children, selected_entity);
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();
    }
    ImGui::EndChild();
}

void Scene::set_graph_children(const std::vector<Entity*>& children, Entity*& selected)
{
    for (int i = 0; i < children.size(); ++i)
    {
        if (children.at(i)->parent == nullptr) continue;
        
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (selected == children.at(i)) node_flags |= ImGuiTreeNodeFlags_Selected;
        if (children.at(i)->children.empty()) node_flags |= ImGuiTreeNodeFlags_Leaf;
        
        const bool opened = ImGui::TreeNodeEx((children.at(i)->name + "##ENTITY" + std::to_string(i)).c_str(), node_flags);
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            selected = children.at(i);

        if (ImGui::BeginPopupContextItem())
        {
            ImGui::SeparatorText("Options");
            ImGui::Button("Delete");
            ImGui::EndPopup();
        }
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            ImGui::SetDragDropPayload("SCENE_DND", &selected, sizeof(Entity*));
            ImGui::Text(selected->name.c_str());
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_DND"))
            {
                IM_ASSERT(payload->DataSize == sizeof(Entity*));
                Entity* payload_entity = *(Entity**)payload->Data;
                if (payload_entity != nullptr) children.at(i)->add_child(payload_entity);
            }
            ImGui::EndDragDropTarget();
        }
        if (opened)
        {
            if (i < children.size() && i >= 0) set_graph_children(children.at(i)->children, selected);
            ImGui::TreePop();
        }

        check_light(children.at(i));
    }
}

void Scene::set_entity_inspector()
{
    ImGui::BeginChild("##EntityInspect", {0, 0}, ImGuiChildFlags_Borders);
    if (selected_entity != nullptr) selected_entity->set_gui();
    ImGui::EndChild();
}

void Scene::set_gui()
{
    ImGui::PushFont(nullptr, ImGui::GetStyle().FontSizeBase * 2.0f);
    ImGui::Text("%s Settings", name.c_str());
    ImGui::PopFont();

    ImGui::TextDisabled("Path: %s", save_path.c_str());

    ImGui::Separator();

    if (ImGui::BeginTable("##SceneSettings", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY))
    {
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Name");
        ImGui::Separator();
        ImGui::TableNextColumn();
        ImGui::InputText("##InputSceneName", name_edit_, std::size(name_edit_));
        name = name_edit_;
        
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Editor camera");
        ImGui::TableNextColumn();
        if (ImGui::Selectable(editor_camera->name.c_str()))
            selected_entity = editor_camera.get();

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Player camera");
        ImGui::TableNextColumn();
        if (ImGui::Button("-")) player_camera = nullptr;
        ImGui::SameLine();
        if (ImGui::Selectable(player_camera == nullptr ? "N/A" : player_camera->name.c_str(), ImGuiSelectableFlags_Highlight))
            selected_entity = player_camera;
        ImGui::SetItemTooltip("Drop entity here");
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_DND"))
            {
                IM_ASSERT(payload->DataSize == sizeof(Entity*));
                Entity* payload_entity = *(Entity**)payload->Data;
                if (payload_entity != nullptr && payload_entity->contains_component<CameraComponent>())
                    player_camera = payload_entity;
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Skybox");
        ImGui::TableNextColumn();
        if (ImGui::Selectable(skybox->name.c_str()))
            selected_entity = skybox.get();

        ImGui::EndTable();
    }
}

void Scene::reset()
{
    entities.clear();
    root_entity->children.clear();
    lights.clear();
    
    player_camera = nullptr;
    current_camera = editor_camera.get();
    selected_entity = nullptr;

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
    std::cout << "SAVED SCENE: " << path << '\n';
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
    std::cout << "LOADED SCENE: " << path << '\n';

    return result;
}

void Scene::serialize(YAML::Emitter& out)
{
    out << YAML::BeginMap;

    out << YAML::Key << "Scene" << YAML::Value << name.c_str();
    if (selected_entity) out << YAML::Key << "Selected Entity Id" << YAML::Value << selected_entity->id;

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
        if (std::cmp_equal(entities.at(i)->id, selected_entity_id))
            selected_entity = entities.at(i).get();
        
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