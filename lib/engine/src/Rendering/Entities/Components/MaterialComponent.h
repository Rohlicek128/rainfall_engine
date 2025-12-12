#pragma once
#include <glm/vec4.hpp>

#include "engine/world/Component.h"
#include "../../Shaders/Program.h"

class MaterialComponent : public Component
{
    float* color_edit_;
public:
    glm::vec4 color;
    float roughness;
    float metallic;

    MaterialComponent(glm::vec4, float = 1.0f, float = 0.0f);
    ~MaterialComponent() override;
    void set_uniforms(Program*);

    std::string get_name() override;
    void set_gui() override;
    void serialize(YAML::Emitter& out) override;
    bool deserialize(YAML::Node& node) override;
};
