#include "CameraComponent.h"

#include <algorithm>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

CameraComponent::CameraComponent(TransformComponent* transform, const ImVec4 clear)
{
    transform_ = transform;
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    yaw = 0.0f;
    pitch = 0.0f;

    fov = 85.0f;
    near_plane = 0.1f;
    far_plane = 1000.0f;

    speed = 10.0f;

    is_wireframe = false;

    clear_color[0] = clear.x;
    clear_color[1] = clear.y;
    clear_color[2] = clear.z;
    clear_color[3] = clear.w;

    gamma = 2.2f;
    threshold = 3.0f;
    key_value = 0.2f;
}

void CameraComponent::move(GLFWwindow* window, const float delta_time)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) transform_->position += speed * delta_time * transform_->rotation;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) transform_->position -= speed * delta_time * transform_->rotation;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) transform_->position -= speed * delta_time * glm::normalize(glm::cross(transform_->rotation, up));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) transform_->position += speed * delta_time * glm::normalize(glm::cross(transform_->rotation, up));

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) transform_->position.y -= speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) transform_->position.y += speed * delta_time;

    transform_->update_pos_edit();
}

void CameraComponent::mouse_move(Mouse& mouse, const float delta_time)
{
    if (mouse.first_move)
    {
        mouse.last_x = mouse.pos_x;
        mouse.last_y = mouse.pos_y;
        mouse.first_move = false;
    }

    const float offset_x = (mouse.pos_x - mouse.last_x) * mouse.sensitivity / 100.0f;
    const float offset_y = (mouse.last_y - mouse.pos_y) * mouse.sensitivity / 100.0f;
    mouse.last_x = mouse.pos_x;
    mouse.last_y = mouse.pos_y;

    set_yaw_pitch(yaw + offset_x, pitch + offset_y);
}

void CameraComponent::set_yaw_pitch(float y, float p)
{
    yaw = y;
    pitch = p;

    pitch = std::min(pitch, 89.0f);
    pitch = std::max(pitch, -89.0f);

    transform_->rotation.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    transform_->rotation.y = sin(glm::radians(pitch));
    transform_->rotation.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    transform_->rotation = glm::normalize(transform_->rotation);

    transform_->update_rot_edit(90.0f);
}

glm::mat4 CameraComponent::get_view_matrix()
{
    return glm::lookAt(transform_->position, transform_->position + transform_->rotation, up);
}

glm::mat4 CameraComponent::get_projection_matrix(const float ratio)
{
    return glm::perspective(glm::radians(fov), ratio, near_plane, far_plane);
}

std::string CameraComponent::get_name()
{
    return "Camera";
}

void CameraComponent::set_gui()
{
    ImGui::SeparatorText("Rendering");
    ImGui::Checkbox("Wireframe", &is_wireframe);
    ImGui::ColorEdit4("Background", clear_color);

    ImGui::SeparatorText("Tone Mapping");
    ImGui::SliderFloat("Gamma", &gamma, 0.0f, 10.0f, "%.1f");
    ImGui::SliderFloat("Threshold", &threshold, 0.0f, 15.0f, "%.2f");
    ImGui::SliderFloat("Key Value", &key_value, 0.0f, 10.0f, "%.3f");
    
    
    ImGui::SeparatorText("Rotation");
    ImGui::PushItemWidth(150);
    ImGui::DragFloat("Yaw", &yaw, 0.1f);
    ImGui::SameLine();
    ImGui::DragFloat("Pitch", &pitch, 0.1f);
    ImGui::PopItemWidth();
    set_yaw_pitch(yaw, pitch);
    
    ImGui::SeparatorText("Field of View");
    ImGui::SliderFloat("##Fov", &fov, 10.0f, 125.0f, "%.1f");

    ImGui::SeparatorText("Clipping Planes");
    ImGui::DragFloat("Near Plane", &near_plane, 0.01f, 0.01f, 100.0f);
    ImGui::DragFloat("Far Plane", &far_plane, 10.0f, 10.0f, 1000000.0f);

    ImGui::SeparatorText("Movement");
    ImGui::DragFloat("Speed", &speed, 0.1f, 0.1f, 500.0f);
}

void CameraComponent::serialize(YAML::Emitter& out)
{
    out << YAML::BeginMap;
    out << YAML::Key << get_name() << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "Enabled" << YAML::Value << is_enabled;
    
    out << YAML::Key << "Wireframe" << YAML::Value << is_wireframe;

    out << YAML::Key << "Yaw" << YAML::Value << yaw;
    out << YAML::Key << "Pitch" << YAML::Value << pitch;
    out << YAML::Key << "Fov" << YAML::Value << fov;
    out << YAML::Key << "Near Plane" << YAML::Value << near_plane;
    out << YAML::Key << "Far Plane" << YAML::Value << far_plane;
    out << YAML::Key << "Speed" << YAML::Value << speed;
    
    out << YAML::Key << "Gamma" << YAML::Value << gamma;
    out << YAML::Key << "Exposure" << YAML::Value << threshold;
    
    out << YAML::Key << "Clear Color" << YAML::Value << YAML::Flow << YAML::BeginSeq;
    out << clear_color[0] << clear_color[1] << clear_color[2] << clear_color[3] << YAML::EndSeq;

    out << YAML::Key << "Up Vector" << YAML::Value;
    emit_out(out, up);
    
    out << YAML::EndMap;
    out << YAML::EndMap;
}

bool CameraComponent::deserialize(YAML::Node& node)
{
    is_enabled = node["Enabled"].as<bool>();
    is_wireframe = node["Wireframe"].as<bool>();

    yaw = node["Yaw"].as<float>();
    pitch = node["Pitch"].as<float>();
    fov = node["Fov"].as<float>();
    near_plane = node["Near Plane"].as<float>();
    far_plane = node["Far Plane"].as<float>();
    speed = node["Speed"].as<float>();
    
    gamma = node["Gamma"].as<float>();
    threshold = node["Exposure"].as<float>();
    
    if (YAML::Node cur = node["Clear Color"])
    {
        const glm::vec4 clear = des_vec4(cur);
        clear_color[0] = clear.x;
        clear_color[1] = clear.y;
        clear_color[2] = clear.z;
        clear_color[3] = clear.w;
    }
    if (YAML::Node cur = node["Up Vector"])
        up = des_vec3(cur);
    
    return true;
}
