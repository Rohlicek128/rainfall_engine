#include "engine/world/components/TransformComponent.h"
#include "engine/world/components/CameraComponent.h"

#include "../../../Utils/MathHelper.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/world/Entity.h"

TransformComponent::TransformComponent(const glm::vec3 pos, const glm::vec3 rot, const glm::vec3 sca)
{
    position = pos;
    rotation = rot;
    scale = sca;

    pos_edit_ = new float[] {position.x, position.y, position.z};
    rot_edit_ = new float[] {rotation.x, rotation.y, rotation.z};
    sca_edit_ = new float[] {scale.x, scale.y, scale.z};
}

glm::mat4 TransformComponent::get_model_matrix()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);
    return model;
}

void TransformComponent::update_pos_edit(const float scaler)
{
    pos_edit_[0] = position.x * scaler;
    pos_edit_[1] = position.y * scaler;
    pos_edit_[2] = position.z * scaler;
}
void TransformComponent::update_rot_edit(const float scaler)
{
    rot_edit_[0] = rotation.x * scaler;
    rot_edit_[1] = rotation.y * scaler;
    rot_edit_[2] = rotation.z * scaler;
}
void TransformComponent::update_sca_edit(const float scaler)
{
    sca_edit_[0] = scale.x * scaler;
    sca_edit_[1] = scale.y * scaler;
    sca_edit_[2] = scale.z * scaler;
}


std::string TransformComponent::get_name()
{
    return "Transform";
}

void TransformComponent::serialize(YAML::Emitter& out)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Transform" << YAML::Value << YAML::BeginMap;

    out << YAML::Key << "Position" << YAML::Value;
    emit_out(out, position);
    out << YAML::Key << "Rotation" << YAML::Value;
    emit_out(out, rotation);
    out << YAML::Key << "Scale" << YAML::Value;
    emit_out(out, scale);

    out << YAML::EndMap;
    out << YAML::EndMap;
}

bool TransformComponent::deserialize(YAML::Node& node)
{
    if (YAML::Node cur = node["Position"])
    {
        position = des_vec3(cur);
        update_pos_edit();
    }
    if (YAML::Node cur = node["Rotation"])
    {
        rotation = des_vec3(cur);
        update_rot_edit();
    }
    if (YAML::Node cur = node["Scale"])
    {
        scale = des_vec3(cur);
        update_sca_edit();
    }

    return true;
}
