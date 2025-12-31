#include "SceneGraphPanel.h"

#include <memory>
#include <engine/world/Scene.h>
#include <engine/world/Entity.h>
#include <engine/world/components/TransformComponent.h>


namespace editor
{
    void SceneGraphPanel::draw(Scene& scene)
    {
        if (ImGui::Begin("Scene Graph ##ENTITIES_GRAPH"))
        {
            if (ImGui::BeginChild("##Entities", ImVec2(0, 0), ImGuiChildFlags_Borders))
            {
                //Scene
                ImGui::PushFont(nullptr, ImGui::GetStyle().FontSizeBase * 2.0f);
                ImGui::SeparatorText(scene.name.c_str());
                ImGui::PopFont();
                if (ImGui::Button("+", {33, 33}))
                {
                    std::unique_ptr<Entity> empty = std::make_unique<Entity>("Empty", new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
                    scene.add_entity(std::move(empty));
                }
                ImGui::SetItemTooltip("Add Empty Entity");
                ImGui::SameLine();
                if (ImGui::Button("-", {33, 33}) && selected_entity != nullptr)
                {
                    scene.remove_entity(selected_entity);
                    selected_entity = scene.entities.back().get();
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
                            scene.root_entity->add_child(payload_entity);
                        }
                    }
                    ImGui::EndDragDropTarget();
                }
                ImGui::SameLine();
                ImGui::TextDisabled("Count: %i", scene.entities.size());

                //Graph
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
                if (ImGui::BeginChild("##EntitiesInner"))
                {
                    draw_graph_children(scene, scene.root_entity->children, selected_entity);
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
            }
            ImGui::EndChild();
        }
        ImGui::End();
    }

    void SceneGraphPanel::draw_graph_children(Scene& scene, const std::vector<Entity*>& children, Entity*& selected)
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
                if (i < children.size() && i >= 0) draw_graph_children(scene, children.at(i)->children, selected);
                ImGui::TreePop();
            }

            scene.check_light(children.at(i));
        }
    }
}
