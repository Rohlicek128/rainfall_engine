#pragma once
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

#include "Component.h"

class Program;
class TransformComponent;

namespace lights
{
    enum class LIGHT_TYPE
    {
        DIRECTIONAL,
        POINT,
        SPOTLIGHT
    };
}

class LightComponent : public Component
{
public:
    lights::LIGHT_TYPE type;
    glm::vec3 color;
    float intensity;
    glm::vec3 attenuation_params;

    LightComponent(const lights::LIGHT_TYPE&, const glm::vec3&, float = 1.0f, const glm::vec3& = glm::vec3(1.0f, 0.5f, 0.3f));
    ~LightComponent() override;
    void set_uniforms(Program*, int, const TransformComponent*);

    glm::mat4 get_light_space_mat(glm::vec3 dir, glm::vec3 cam_pos, float zoom, float near, float far);

    std::string get_name() override;
    void serialize(YAML::Emitter& out) override;
    bool deserialize(YAML::Node& node) override;
};
