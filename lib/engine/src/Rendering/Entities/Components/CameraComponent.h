#pragma once
#include "Component.h"
#include "TransformComponent.h"
#include "../Mouse.h"

#include <glfw3.h>


class CameraComponent : public Component
{
    TransformComponent* transform_;

public:
    glm::vec3 up;

    float yaw, pitch;
    float fov;
    float near_plane;
    float far_plane;
    float speed;
    float clear_color[4];

    float gamma;
    float threshold;
    float key_value;

    bool is_wireframe;

    CameraComponent(TransformComponent*, ImVec4 = {0.1f, 0.1f, 0.1f, 1.0f});
    ~CameraComponent() override = default;

    void move(GLFWwindow*, float);
    void mouse_move(Mouse&, float);
    void set_yaw_pitch(float, float);
    glm::mat4 get_view_matrix();
    glm::mat4 get_projection_matrix(float);

    std::string get_name() override;
    void set_gui() override;
    void serialize(YAML::Emitter& out) override;
    bool deserialize(YAML::Node& node) override;
};
