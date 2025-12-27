#include "engine/world/components/MaterialComponent.h"

#include "../../Shaders/Program.h"

MaterialComponent::MaterialComponent(const glm::vec4 color, const float roughness, const float metallic)
{
    this->color = color;
    this->metallic = metallic;
    this->roughness = roughness;

    color_edit_ = new float[] {color.r, color.g, color.b, color.a};
}

MaterialComponent::~MaterialComponent()
{
    //delete[] color_edit_;
}

void MaterialComponent::set_uniforms(Program* program)
{
    program->set_uniform("material.color", glm::vec3(color));
    program->set_uniform("material.roughness", roughness);
    program->set_uniform("material.metallic", metallic);
    program->set_uniform("is_light", 0);
}

std::string MaterialComponent::get_name()
{
    return "Material";
}


void MaterialComponent::serialize(YAML::Emitter& out)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Material" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "Enabled" << YAML::Value << is_enabled;

    out << YAML::Key << "Color" << YAML::Value;
    emit_out(out, color);
    out << YAML::Key << "Roughness" << YAML::Value << roughness;
    out << YAML::Key << "Metallic" << YAML::Value << metallic;

    out << YAML::EndMap;
    out << YAML::EndMap;
}

bool MaterialComponent::deserialize(YAML::Node& node)
{
    is_enabled = node["Enabled"].as<bool>();

    if (YAML::Node cur = node["Color"])
    {
        color = des_vec4(cur);
        color_edit_[0] = color.r;
        color_edit_[1] = color.g;
        color_edit_[2] = color.b;
        color_edit_[3] = color.a;
    }
    roughness = node["Roughness"].as<float>();
    metallic = node["Metallic"].as<float>();

    return true;
}
