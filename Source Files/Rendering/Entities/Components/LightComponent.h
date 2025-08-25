#pragma once
#include <glm/vec3.hpp>

#include "Component.h"
#include "../../Shaders/Program.h"

enum light_type
{
    DIRECTIONAL,
    POINT
};

class LightComponent : public Component
{
    int type_edit_;
    float* amb_edit_;
    float* dif_edit_;
    float* spe_edit_;
    bool uniform_color_;
    
public:
    light_type type;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float intensity;
    glm::vec3 attenuation_params;
    
    LightComponent(const light_type&, const glm::vec3&, const glm::vec3&, const glm::vec3&, float = 1.0f, const glm::vec3& = glm::vec3(1.0f, 0.14f, 0.07f));
    ~LightComponent() override;
    void set_uniforms(Program*, int, const TransformComponent*);
    void set_gui() override;
};
