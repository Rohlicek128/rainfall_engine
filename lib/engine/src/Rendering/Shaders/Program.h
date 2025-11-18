#pragma once

#include "../Buffers/IBindable.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/mat3x3.hpp"

#include "engine/world/Scene.h"
#include "Shader.h"

class Program : public IBindable
{
public:
    Program(const std::vector<Shader>&);
    ~Program() override;
    void is_successful();
    void bind() override;
    void unbind() override;

    void set_uniform(const char*, int);
    void set_uniform(const char*, float);
    void set_uniform(const char*, const glm::vec3&);
    void set_uniform(const char*, float, float, float);
    void set_uniform(const char*, const glm::mat3&);
    void set_uniform(const char*, const glm::vec4&);
    void set_uniform(const char*, const glm::mat4&);
};
