#include "LightComponent.h"

#include <algorithm>
#include <string>

LightComponent::LightComponent(const light_type& t, const glm::vec3& a, const glm::vec3& d, const glm::vec3& s, const float i, const glm::vec3& attenuation)
{
    type = t;
    ambient = a;
    diffuse = d;
    specular = s;
    intensity = i;
    attenuation_params = attenuation;

    type_edit_ = type;
    amb_edit_ = new float[] {ambient.x, ambient.y, ambient.z};
    dif_edit_ = new float[] {diffuse.x, diffuse.y, diffuse.z};
    spe_edit_ = new float[] {specular.x, specular.y, specular.z};
    switch (t)
    {
        case DIRECTIONAL:
            uniform_color_ = false; break;
        case POINT:
            ambient = diffuse;
            specular = diffuse;
            uniform_color_ = true; break;
    }
}

LightComponent::~LightComponent()
{
    delete[] amb_edit_;
    delete[] dif_edit_;
    delete[] spe_edit_;
}

void LightComponent::set_uniforms(Program* program, const int index, const TransformComponent* transform)
{
    const std::string dir_lights = "dir_lights[" + std::to_string(index);
    const std::string point_lights = "point_lights[" + std::to_string(index);
    switch (type)
    {
        case DIRECTIONAL:
            program->set_uniform((dir_lights + "].is_lighting").c_str(), is_enabled);
            program->set_uniform((dir_lights + "].direction").c_str(), transform->rotation);
            program->set_uniform((dir_lights + "].ambient").c_str(), ambient);
            program->set_uniform((dir_lights + "].diffuse").c_str(), diffuse * intensity);
            program->set_uniform((dir_lights + "].specular").c_str(), specular);
            break;
        case POINT:
            program->set_uniform((point_lights + "].is_lighting").c_str(), is_enabled);
            program->set_uniform((point_lights + "].position").c_str(), transform->position);
            program->set_uniform((point_lights + "].ambient").c_str(), ambient * intensity);
            program->set_uniform((point_lights + "].diffuse").c_str(), diffuse * intensity);
            program->set_uniform((point_lights + "].specular").c_str(), specular * intensity);
            program->set_uniform((point_lights + "].attenuation_params").c_str(), attenuation_params);
            break;
    }
}

void LightComponent::set_gui()
{
    const char* type_names[] = { "Directional", "Point" };
    ImGui::Combo("Type", &type_edit_, type_names, std::size(type_names));
    type = (light_type)type_edit_;

    ImGui::SeparatorText("Color");
    ImGui::Checkbox("Uniform color edit", &uniform_color_);

    if (uniform_color_)
    {
        ImGui::ColorEdit3("Color", dif_edit_);
        ambient.x = dif_edit_[0];
        ambient.y = dif_edit_[1];
        ambient.z = dif_edit_[2];
        diffuse.x = dif_edit_[0];
        diffuse.y = dif_edit_[1];
        diffuse.z = dif_edit_[2];
        specular.x = dif_edit_[0];
        specular.y = dif_edit_[1];
        specular.z = dif_edit_[2];
    }
    else
    {
        ImGui::ColorEdit3("Ambient", amb_edit_);
        ambient.x = amb_edit_[0];
        ambient.y = amb_edit_[1];
        ambient.z = amb_edit_[2];
    
        ImGui::ColorEdit3("Diffuse", dif_edit_);
        diffuse.x = dif_edit_[0];
        diffuse.y = dif_edit_[1];
        diffuse.z = dif_edit_[2];
    
        ImGui::ColorEdit3("Specular", spe_edit_);
        specular.x = spe_edit_[0];
        specular.y = spe_edit_[1];
        specular.z = spe_edit_[2];
    }

    if (ImGui::Button("R")) intensity = 1.0f;
    ImGui::SameLine();
    ImGui::DragFloat("Intensity", &intensity, 0.01f, 0, 0, "%.1f");
    intensity = std::max(intensity, 0.0f);

    if (type == POINT)
    {
        ImGui::SeparatorText("Attenuation");
        ImGui::PushItemWidth(200);
        ImGui::DragFloat("Constant", &attenuation_params.x, 0.01f, 0, 0, "%.6f");
        ImGui::DragFloat("Linear", &attenuation_params.y, 0.001f, 0, 0, "%.6f");
        ImGui::DragFloat("Quadratic", &attenuation_params.z, 0.0001f, 0, 0, "%.6f");
        ImGui::PopItemWidth();
    }
}
