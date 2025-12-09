#include "MeshComponent.h"

#include <iterator>

#include "engine/managers/Mesh.h"

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

std::string MeshComponent::get_name()
{
    return "Mesh";
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
    //ImGui::Checkbox("Is Shaded", &is_shaded);
    ImGui::Checkbox("Is Culled", &is_culled);
    ImGui::Checkbox("Is Inverted", &is_inverted);

    const char* type_names[] = { "Points", "Lines", "Line Loop", "Line Strip", "Triangles", "Triangle Strip", "Triangle Fan", "Quads", "Quads Strip", "Polygon" };
    ImGui::Combo("Primitive", &primitive_type, type_names, std::size(type_names));
}

void MeshComponent::serialize(YAML::Emitter& out)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Mesh" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "Enabled" << YAML::Value << is_enabled;

    out << YAML::Key << "Model Id" << YAML::Value << model_index;
    out << YAML::Key << "Primitive" << YAML::Value << primitive_type;
    out << YAML::Key << "Is Culled" << YAML::Value << is_culled;
    out << YAML::Key << "Is Inverted" << YAML::Value << is_inverted;

    out << YAML::EndMap;
    out << YAML::EndMap;
}

bool MeshComponent::deserialize(YAML::Node& node)
{
    is_enabled = node["Enabled"].as<bool>();

    model_index = node["Model Id"].as<int>();
    primitive_type = node["Primitive"].as<int>();
    is_culled = node["Is Culled"].as<bool>();
    is_inverted = node["Is Inverted"].as<bool>();

    return true;
}
