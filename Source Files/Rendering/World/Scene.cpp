#include "Scene.h"

#include "../Entities/Entity.h"
#include "../World/Mesh.h"

#include "../Entities/Components/CameraComponent.h"
#include "../Entities/Components/TextureComponent.h"
#include "../Entities/Components/MeshComponent.h"

Scene::Scene(const std::string& name, Mesh&& mesh)
{
    root_entity = std::make_unique<Entity>("__root", new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f) ,glm::vec3(0.0f)));
    root_entity->is_root = true;

    editor_camera = std::make_unique<Entity>("__editor_camera",
        new TransformComponent(glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f))
        );
    editor_camera->add_component(CAMERA, new CameraComponent(editor_camera->transform, {0.25f, 0.25f, 0.25f, 1.0f}));
    player_camera = nullptr;

    skybox = std::make_unique<Entity>("__skybox",
        new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f))
        );
    skybox->add_component(MESH, new MeshComponent(0, GL_TRIANGLES, &mesh));
    skybox->add_component(TEXTURE, new TextureComponent(0x8513, 1, 1));
    
    this->name = name;
    this->mesh = std::make_unique<Mesh>(std::move(mesh));

    opened_gui = false;
    selected_entity = nullptr;
}

void Scene::add_entity(std::unique_ptr<Entity> entity, const bool is_light)
{
    if (entity->parent == nullptr) root_entity->add_child(entity.get());
    entities.push_back(std::move(entity));
    entities.back()->set_mesh_to_component(mesh.get());
    if (is_light) add_light(entities.back().get());
}

void Scene::remove_entity(Entity* entity)
{
    for (int i = 0; i < entities.size(); ++i)
    {
        if (entity == entities.at(i).get())
        {
            entity->parent->remove_child(entity);
            for (Entity* child : entities.at(i)->children)
                child->parent = root_entity.get();
            
            root_entity->remove_child(entity);
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

Entity* Scene::find_entity_by_id(const unsigned int id)
{
    for (int i = 0; i < entities.size(); ++i)
    {
        if (entities.at(i)->id == id) return entities.at(i).get();
    }
    return nullptr;
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

    ImGui::Separator();

    if (ImGui::BeginTable("##SceneSettings", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY))
    {
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f);

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
                if (payload_entity != nullptr && payload_entity->component_exists(CAMERA))
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
