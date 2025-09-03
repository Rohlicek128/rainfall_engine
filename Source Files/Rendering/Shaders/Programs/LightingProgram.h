#pragma once
#include "../Program.h"
#include "../../Buffers/Screen/GBuffer.h"

class LightingProgram : public Program
{
public:
    LightingProgram(const std::vector<Shader>&);
    void draw(const Scene&, Mesh&, int, GBuffer&);
};
