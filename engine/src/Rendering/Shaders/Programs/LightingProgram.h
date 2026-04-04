#pragma once
#include "../Program.h"

class ShadowMap;
class GBuffer;
namespace lights
{
    enum class LIGHT_TYPE : int;
}


class LightingProgram : public Program
{
    float* ambient_edit_;

    void set_lights_uniforms(const Scene&, lights::LIGHT_TYPE, unsigned int);
public:
    static constexpr unsigned int nr_directional_lights = 1;
    static constexpr unsigned int nr_point_lights = 5;

    glm::vec3 ambient;

    LightingProgram(const std::vector<Shader>&);
    ~LightingProgram() override = default;

    void draw(const Scene&, Mesh&, int, GBuffer&, ShadowMap&);
};
