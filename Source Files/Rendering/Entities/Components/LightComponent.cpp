#include "LightComponent.h"
#include "../../Shaders/Program.h"
#include "../../Entities/Components/TransformComponent.h"

#include <algorithm>
#include <string>

LightComponent::LightComponent(const light_type& t, const glm::vec3& c, const float i, const glm::vec3& attenuation)
{
    type = t;
    color = c;
    intensity = i;
    attenuation_params = attenuation;

    type_edit_ = type;
    color_edit_ = new float[] {color.x, color.y, color.z};
}

LightComponent::~LightComponent()
{
    delete[] color_edit_;
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
            program->set_uniform((dir_lights + "].color").c_str(), color * intensity);
            break;
        case POINT:
            program->set_uniform((point_lights + "].is_lighting").c_str(), is_enabled);
            program->set_uniform((point_lights + "].position").c_str(), transform->position);
            program->set_uniform((point_lights + "].color").c_str(), color * intensity);
            program->set_uniform((point_lights + "].attenuation_params").c_str(), attenuation_params);
            break;
    }
}

void LightComponent::set_gui()
{
    const char* type_names[] = { "Directional", "Point", "Spotlight" };
    ImGui::Combo("Type", &type_edit_, type_names, std::size(type_names));
    type = (light_type)type_edit_;

    ImGui::SeparatorText("Color");

    ImGui::ColorEdit3("Color", color_edit_);
    color.x = color_edit_[0];
    color.y = color_edit_[1];
    color.z = color_edit_[2];
    

    if (ImGui::Button("R")) intensity = 1.0f;
    ImGui::SameLine();
    ImGui::DragFloat("Intensity", &intensity, 0.01f, 0, 0, "%.1f");
    intensity = std::max(intensity, 0.0f);

    if (type == POINT)
    {
        ImGui::SeparatorText("Attenuation");
        ImGui::PushItemWidth(200);
        ImGui::DragFloat("Constant", &attenuation_params.x, 0.01f, 0, 0, "%.4f");
        ImGui::DragFloat("Linear", &attenuation_params.y, 0.001f, 0, 0, "%.4f");
        ImGui::DragFloat("Quadratic", &attenuation_params.z, 0.0001f, 0, 0, "%.4f");
        ImGui::PopItemWidth();
    }
}
