#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Component.h"

class Entity;
class CameraComponent;

class TransformComponent : public Component
{
    float* pos_edit_;
    float* rot_edit_;
    float* sca_edit_;
    
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    
    TransformComponent(glm::vec3, glm::vec3, glm::vec3);
    ~TransformComponent() override = default;
    glm::mat4 get_model_matrix();
    void update_pos_edit(float = 1.0f);
    void update_rot_edit(float = 1.0f);
    void update_sca_edit(float = 1.0f);
    void set_guizmo(const Entity*, int);
    void set_gui() override;
};
