#include "ISerializable.h"

YAML::Emitter& ISerializable::emit_out(YAML::Emitter& out, const glm::vec3& value)
{
    out << YAML::Flow << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
    return out;
}

YAML::Emitter& ISerializable::emit_out(YAML::Emitter& out, const glm::vec4& value)
{
    out << YAML::Flow << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
    return out;
}

glm::vec3 ISerializable::des_vec3(YAML::Node& node)
{
    if (!node.IsSequence()) return glm::vec3(0.0f);
    
    glm::vec3 value;

    YAML::iterator cur_node = node.begin();
    value.x = cur_node->as<float>();
    value.y = (++cur_node)->as<float>();
    value.z = (++cur_node)->as<float>();

    return value;
}

glm::vec4 ISerializable::des_vec4(YAML::Node& node)
{
    glm::vec4 value = glm::vec4(0.0f);

    YAML::const_iterator cur_node = node.begin();
    value.x = cur_node->as<float>();
    value.y = (++cur_node)->as<float>();
    value.z = (++cur_node)->as<float>();
    value.w = (++cur_node)->as<float>();

    return value;
}