#include "EntityInspectorPanel.h"
#include "../inspectors/ElementInspector.h"

#include "engine/src/Rendering/Buffers/Textures/TextureManager.h"
#include "engine/world/components/MeshComponent.h"
#include "engine/world/components/CameraComponent.h"
#include "engine/world/components/MaterialComponent.h"
#include "engine/world/components/TextureComponent.h"
#include "engine/world/components/LightComponent.h"

#include <engine/world/Entity.h>


namespace editor
{
    EntityInspectorPanel::EntityInspectorPanel()
    {
        add_selected_ = -1;
    }


    void EntityInspectorPanel::draw(Entity* entity, TextureManager& manager)
    {
        if (ImGui::Begin("Inspector ##ENTITIES_EDITOR") && entity)
        {
            ImGui::PushFont(nullptr, ImGui::GetStyle().FontSizeBase * 2.0f);
            ImGui::Selectable(entity->name.c_str());
            ImGui::PopFont();
            if (ImGui::BeginPopupContextItem())
            {
                char name_edit[64] = "";
                entity->name.copy(name_edit, std::size(name_edit));

                ImGui::InputText("##InputName", name_edit, std::size(name_edit));
                ImGui::SetNextItemShortcut(ImGuiKey_Enter);
                if (ImGui::Button("Rename"))
                {
                    entity->name = name_edit;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
            ImGui::SetItemTooltip("Right-click to Rename");


            ImGui::TextDisabled("ID: %i", entity->id);
            ImGui::Separator();

            ImGui::Checkbox("Is Visible", &entity->is_visible);


            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ElementInspector::draw_transform_component(*entity->transform);
            }
            if (MeshComponent* mesh = component_header<MeshComponent>(entity))
            {
                ElementInspector::draw_mesh_component(*mesh);
            }
            if (CameraComponent* camera = component_header<CameraComponent>(entity))
            {
                ElementInspector::draw_camera_component(*camera);
            }
            if (TextureComponent* texture = component_header<TextureComponent>(entity))
            {
                ElementInspector::draw_texture_component(*texture, manager);
            }
            if (MaterialComponent* material = component_header<MaterialComponent>(entity))
            {
                ElementInspector::draw_material_component(*material);
            }
            if (LightComponent* light = component_header<LightComponent>(entity))
            {
                ElementInspector::draw_light_component(*light);
            }


            //Add Component
            ImGui::Separator();
            if (ImGui::BeginMenu("Add Component"))
            {
                ImGui::SeparatorText("Select a Component:");
                ImGui::InputTextWithHint("##Search", "Search", component_search_, std::size(component_search_));

                if (ImGui::BeginChild("##SelectComponent", ImVec2(-FLT_MIN, 7 * ImGui::GetTextLineHeightWithSpacing()), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY))
                {
                    for (int i = 1; i < 6; ++i)
                    {
                        if (entity->contains_component_enum((COMPONENTS_IDS)i)) continue;
                        if (strcmp(component_search_, "") != 0 &&
                            !check_search_string(to_string((COMPONENTS_IDS)i), component_search_, std::size(component_search_)))
                            continue;

                        if (ImGui::Selectable(to_string((COMPONENTS_IDS)i), add_selected_ == i)) add_selected_ = i;
                        if (add_selected_ == i) ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndChild();
                }

                ImGui::EndMenu();
            }

            if (add_selected_ != -1)
            {
                switch ((COMPONENTS_IDS)add_selected_)
                {
                    case TRANSFORM: entity->add_component<TransformComponent>(glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0)); break;
                    case MESH: entity->add_component<MeshComponent>(0); break;
                    case CAMERA: entity->add_component<CameraComponent>(entity->transform); break;
                    case MATERIAL: entity->add_component<MaterialComponent>(glm::vec4(1.0)); break;
                    case TEXTURE: entity->add_component<TextureComponent>(GL_TEXTURE_2D); break;
                    case LIGHT: entity->add_component<LightComponent>(lights::LIGHT_TYPE::POINT, glm::vec3(1.0f)); break;
                }
                add_selected_ = -1;
            }
        }
        ImGui::End();
    }

    bool EntityInspectorPanel::check_search_string(const char* whole, const char* part, const int part_lenght)
    {
        for (int i = 0; i < part_lenght; ++i)
        {
            if (part[i] == '\0') break;
            if (tolower(part[i]) != tolower(whole[i]) ) return false;
        }
        return true;
    }
}
