#pragma once
#include "../Program.h"
#include "../../Buffers/Screen/GBuffer.h"

enum LIGHT_TYPE : int;
class LightingProgram : public Program
{
    void set_lights_uniforms(const Scene&, LIGHT_TYPE, unsigned int);
public:
    static constexpr unsigned int nr_directional_lights = 1;
    static constexpr unsigned int nr_point_lights = 5;

    glm::vec3 ambient;
    
    LightingProgram(const std::vector<Shader>&);
    void draw(const Scene&, Mesh&, int, GBuffer&);
};
