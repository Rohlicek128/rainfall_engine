#include "engine/world/components/MeshComponent.h"

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
