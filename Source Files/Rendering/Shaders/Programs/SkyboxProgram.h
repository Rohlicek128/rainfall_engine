#pragma once
#include "../Program.h"

class SkyboxProgram : public Program
{
public:
    SkyboxProgram(const std::vector<Shader>&);
    void draw(const Scene& scene, float aspect_ratio);
};
