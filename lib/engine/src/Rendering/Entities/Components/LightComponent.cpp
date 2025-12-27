#include "engine/world/components/LightComponent.h"
#include "../../Shaders/Program.h"
#include "engine/world/components/TransformComponent.h"


#include <string>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

LightComponent::LightComponent(const lights::LIGHT_TYPE& t, const glm::vec3& c, const float i, const glm::vec3& attenuation)
{
    type = t;
    color = c;
    intensity = i;
    attenuation_params = attenuation;

    type_edit_ = static_cast<int>(type);
    color_edit_ = new float[] {color.x, color.y, color.z};
}

LightComponent::~LightComponent()
{
    //delete[] color_edit_;
}

void LightComponent::set_uniforms(Program* program, const int index, const TransformComponent* transform)
{
    const std::string dir_lights = "dir_lights[" + std::to_string(index);
    const std::string point_lights = "point_lights[" + std::to_string(index);
    switch (type)
    {
        case lights::LIGHT_TYPE::DIRECTIONAL:
            program->set_uniform((dir_lights + "].is_lighting").c_str(), is_enabled);
            program->set_uniform((dir_lights + "].direction").c_str(), transform->rotation);
            program->set_uniform((dir_lights + "].color").c_str(), color * intensity);
            break;
        case lights::LIGHT_TYPE::POINT:
            program->set_uniform((point_lights + "].is_lighting").c_str(), is_enabled);
            program->set_uniform((point_lights + "].position").c_str(), transform->position);
            program->set_uniform((point_lights + "].color").c_str(), color * intensity);
            program->set_uniform((point_lights + "].attenuation_params").c_str(), attenuation_params);
            break;
        case lights::LIGHT_TYPE::SPOTLIGHT:
            break;
    }
}

glm::mat4 LightComponent::get_light_space_mat(const glm::vec3 dir, const glm::vec3 cam_pos, const float zoom, const float near, const float far)
{
    if (type != lights::LIGHT_TYPE::DIRECTIONAL) return {0.0f};

    const glm::mat4 projection = glm::ortho(-zoom, zoom, -zoom, zoom, near, far);
    const glm::mat4 view = glm::lookAt(-dir + cam_pos, cam_pos, glm::vec3(0.0f, 1.0f, 0.0f));

    return projection * view;
}

std::string LightComponent::get_name()
{
    return "Light";
}


void LightComponent::serialize(YAML::Emitter& out)
{
    out << YAML::BeginMap;
    out << YAML::Key << get_name() << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "Enabled" << YAML::Value << is_enabled;

    out << YAML::Key << "Type" << YAML::Value << static_cast<int>(type);
    out << YAML::Key << "Color" << YAML::Value;
    emit_out(out, color);
    out << YAML::Key << "Intensity" << YAML::Value << intensity;
    if (type == lights::LIGHT_TYPE::POINT)
    {
        out << YAML::Key << "Attenuation" << YAML::Value;
        emit_out(out, attenuation_params);
    }

    out << YAML::EndMap;
    out << YAML::EndMap;
}

bool LightComponent::deserialize(YAML::Node& node)
{
    is_enabled = node["Enabled"].as<bool>();

    type = (lights::LIGHT_TYPE)node["Type"].as<int>();
    type_edit_ = static_cast<int>(type);
    if (YAML::Node cur = node["Color"])
    {
        color = des_vec3(cur);
        color_edit_[0] = color.x;
        color_edit_[1] = color.y;
        color_edit_[2] = color.z;
    }

    intensity = node["Intensity"].as<float>();
    if (type == lights::LIGHT_TYPE::POINT)
    {
        if (YAML::Node cur = node["Attenuation"])
            attenuation_params = des_vec3(cur);
    }

    return true;
}
