#include "MeshComponent.h"

#include <iterator>

#include "../../World/Mesh.h"

MeshComponent::MeshComponent(const int index, const int type, Mesh* mesh)
{
    model_index = index;
    
    primitive_type = type;
    is_inverted = false;
    is_culled = true;
    is_shaded = true;

    mesh_ = mesh;
    is_mesh_view_open_ = false;
}

void MeshComponent::set_mesh_manager(Mesh*& mesh)
{
    mesh_ = mesh;
}

void MeshComponent::set_gui()
{
    ImGui::SeparatorText("Model");
    if (const ModelData* model_data = mesh_->get_model(model_index))
    {
        if (ImGui::Selectable(("  " + model_data->name).c_str(), true, 0, {ImGui::GetContentRegionAvail().x * 0.5f, 0}))
            is_mesh_view_open_ = true;
        ImGui::SetItemTooltip("Click to Browse");
    }
    //ImGui::SameLine();
    //if (ImGui::Button("Change..")) is_mesh_view_open_ = true;
    
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (is_mesh_view_open_)
    {
        if (ImGui::Begin("Select a Mesh", &is_mesh_view_open_, ImGuiWindowFlags_NoDocking))
        {
            const int selected = mesh_->select_mesh_gui();
            if (selected != -1)
            {
                model_index = selected;
                is_mesh_view_open_ = false;
            }
        }
        ImGui::End();
    }

    ImGui::SeparatorText("Rendering");
    ImGui::Checkbox("Is Shaded", &is_shaded);
    ImGui::Checkbox("Is Culled", &is_culled);
    ImGui::Checkbox("Is Inverted", &is_inverted);
    
    const char* type_names[] = { "Points", "Lines", "Line Loop", "Line Strip", "Triangles", "Triangle Strip", "Triangle Fan", "Quads", "Quads Strip", "Polygon" };
    ImGui::Combo("Primitive", &primitive_type, type_names, std::size(type_names));
}
