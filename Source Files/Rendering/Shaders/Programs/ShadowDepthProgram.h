#pragma once
#include "../Program.h"

class ShadowDepthProgram : public Program
{
public:
    ShadowDepthProgram(const std::vector<Shader>& shaders);
    void draw(const Scene& scene, const glm::mat4& light_space);
};
