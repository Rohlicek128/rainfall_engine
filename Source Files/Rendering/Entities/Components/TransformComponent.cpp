#include "TransformComponent.h"
#include "CameraComponent.h"
#include  "../../../Imgui/ImGuizmo.h"
#include "../../../Utils/MathHelper.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Entity.h"

TransformComponent::TransformComponent(const glm::vec3 pos, const glm::vec3 rot, const glm::vec3 sca)
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

void TransformComponent::set_guizmo(const Entity* camera, const int operation)
{
    if (operation == -1) return;
    
    ImGuizmo::SetDrawlist();
    
    const ImVec2 size = ImGui::GetWindowSize();
    
    const glm::mat4& projection = dynamic_cast<CameraComponent*>(camera->components->at(CAMERA))->get_projection_matrix(size.x / size.y);
    const glm::mat4& view = dynamic_cast<CameraComponent*>(camera->components->at(CAMERA))->get_view_matrix(); // glm::inverse(camera->transform->get_model_matrix())
    glm::mat4 transform = get_model_matrix();

    ImGuizmo::Enable(true);
    ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
        (ImGuizmo::OPERATION)operation, ImGuizmo::LOCAL, glm::value_ptr(transform));

    if (ImGuizmo::IsUsing())
    {
        glm::vec3 tra, rot, sca;
        MathHelper::decompose(transform, tra, rot, sca);
        
        this->position = tra;
        this->rotation += rot - this->rotation;
        this->scale = sca;
    }
    
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
