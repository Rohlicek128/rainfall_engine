#pragma once
#include <glm/detail/type_mat2x2.hpp>

class MathHelper
{
public:
    static bool decompose(glm::mat4 const& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
};
