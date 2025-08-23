#pragma once
#include <glm/vec4.hpp>

#include "Component.h"
#include "../../Shaders/Program.h"

class MaterialComponent : public Component
{
    float* color_edit_;
public:
    glm::vec4 color;
    float shininess;
    
    MaterialComponent(glm::vec4, float = 32.0f);
    ~MaterialComponent() override = default;
    void set_uniforms(Program*);
    void set_gui() override;
};
