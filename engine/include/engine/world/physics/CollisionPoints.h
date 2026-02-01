#pragma once

#include "glm/ext/vector_float3.hpp"


namespace engine::physics
{
    struct CollisionsPoints
    {
        bool has_collision;
        glm::vec3 a;
        glm::vec3 b;
        glm::vec3 normal;
        float depth;
    };
}
