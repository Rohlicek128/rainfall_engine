#pragma once

#define YAML_CPP_STATIC_DEFINE
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <yaml-cpp/yaml.h>

class ISerializable
{
public:
    virtual ~ISerializable() = default;
    YAML::Emitter& emit_out(YAML::Emitter&, const glm::vec3&);
    YAML::Emitter& emit_out(YAML::Emitter&, const glm::vec4&);
    glm::vec3 des_vec3(YAML::Node&);
    glm::vec4 des_vec4(YAML::Node&);

    virtual void serialize(YAML::Emitter& out) = 0;
    virtual bool deserialize(YAML::Node& node) = 0;
};
