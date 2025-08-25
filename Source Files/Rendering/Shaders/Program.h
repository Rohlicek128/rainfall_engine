#pragma once
#include "Shader.h"
#include "../Buffers/Mesh.h"
#include "../Buffers/Textures/Texture.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/mat3x3.hpp"

class Program : public IBindable
{
public:
    Program(const Shader*);
    ~Program() override;
    void bind() override;
    void unbind() override;

    void draw(Mesh*, const std::vector<std::shared_ptr<Entity>>&, const Entity*, int, int);
    void draw_screen(Mesh*, unsigned int);
    
    void set_uniform(const char*, int);
    void set_uniform(const char*, float);
    void set_uniform(const char*, const glm::vec3&);
    void set_uniform(const char*, float, float, float);
    void set_uniform(const char*, const glm::mat3&);
    void set_uniform(const char*, const glm::vec4&);
    void set_uniform(const char*, const glm::mat4&);
};