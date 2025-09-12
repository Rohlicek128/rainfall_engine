#pragma once
#include <glm/vec3.hpp>

#include "Component.h"

class Program;
class TransformComponent;

enum light_type
{
    DIRECTIONAL,
    POINT,
    SPOTLIGHT
};

class LightComponent : public Component
{
    int type_edit_;
    float* color_edit_;
    
public:
    light_type type;
    glm::vec3 color;
    float intensity;
    glm::vec3 attenuation_params;
    
    LightComponent(const light_type&, const glm::vec3&, float = 1.0f, const glm::vec3& = glm::vec3(1.0f, 0.05f, 0.9f));
    ~LightComponent() override;
    void set_uniforms(Program*, int, const TransformComponent*);
    void set_gui() override;
};
