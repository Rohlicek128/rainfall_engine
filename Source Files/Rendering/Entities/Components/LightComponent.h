#pragma once
#include <glm/vec3.hpp>

#include "Component.h"

class Program;
class TransformComponent;

enum LIGHT_TYPE
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
    LIGHT_TYPE type;
    glm::vec3 color;
    float intensity;
    glm::vec3 attenuation_params;
    
    LightComponent(const LIGHT_TYPE&, const glm::vec3&, float = 1.0f, const glm::vec3& = glm::vec3(1.0f, 0.5f, 0.3f));
    ~LightComponent() override;
    void set_uniforms(Program*, int, const TransformComponent*);

    std::string get_name() override;
    void set_gui() override;
    void serialize(YAML::Emitter& out) override;
    bool deserialize(YAML::Node& node) override;
};
