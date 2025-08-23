#include "TransformComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TransformComponent::TransformComponent(glm::vec3 pos, glm::vec3 rot, glm::vec3 sca)
{
    position = pos;
    rotation = rot;
    scale = sca;

    pos_edit_ = new float[] {position.x, position.y, position.z};
    rot_edit_ = new float[] {rotation.x, rotation.y, rotation.z};
    sca_edit_ = new float[] {scale.x, scale.y, scale.z};
}

glm::mat4 TransformComponent::get_model_matrix()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);
    return model;
}

void TransformComponent::update_pos_edit(const float scaler)
{
    pos_edit_[0] = position.x * scaler;
    pos_edit_[1] = position.y * scaler;
    pos_edit_[2] = position.z * scaler;
}
void TransformComponent::update_rot_edit(const float scaler)
{
    rot_edit_[0] = rotation.x * scaler;
    rot_edit_[1] = rotation.y * scaler;
    rot_edit_[2] = rotation.z * scaler;
}
void TransformComponent::update_sca_edit(const float scaler)
{
    sca_edit_[0] = scale.x * scaler;
    sca_edit_[1] = scale.y * scaler;
    sca_edit_[2] = scale.z * scaler;
}

void TransformComponent::set_gui()
{
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat3("Position", pos_edit_, 0.01f);
        position.x = pos_edit_[0];
        position.y = pos_edit_[1];
        position.z = pos_edit_[2];
    
        ImGui::DragFloat3("Rotation", rot_edit_, 0.1f);
        rotation.x = rot_edit_[0];
        rotation.y = rot_edit_[1];
        rotation.z = rot_edit_[2];
    
        ImGui::DragFloat3("Scale", sca_edit_, 0.01f);
        scale.x = sca_edit_[0];
        scale.y = sca_edit_[1];
        scale.z = sca_edit_[2];
    }
}
