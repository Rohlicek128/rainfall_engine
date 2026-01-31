#pragma once
#include "../Program.h"

class GeometryProgram : public Program
{
public:
    GeometryProgram(const std::vector<Shader>&);
    void draw(const Scene&, float);
};
