#include "MaterialComponent.h"

MaterialComponent::MaterialComponent(const glm::vec4 color, const float shininess)
{
    this->color = color;
    this->shininess = shininess;

    color_edit_ = new float[] {color.r, color.g, color.b, color.a};
}

void MaterialComponent::set_uniforms(Program* program)
{
    program->set_uniform("material.color", glm::vec3(color));
    program->set_uniform("material.shininess", shininess);
    program->set_uniform("is_light", 0);
}

void MaterialComponent::set_gui()
{
    ImGui::Checkbox("Enabled ##MATERIAL", &is_enabled);
    if (!is_enabled) ImGui::BeginDisabled();
        
    ImGui::ColorEdit4("Color", color_edit_);
    color.r = color_edit_[0];
    color.g = color_edit_[1];
    color.b = color_edit_[2];
    color.a = color_edit_[3];

    ImGui::SliderFloat("Shininess", &shininess, 1.0f, 1024.0f, "%.1f", ImGuiSliderFlags_Logarithmic);

    if (!is_enabled) ImGui::EndDisabled();
}
