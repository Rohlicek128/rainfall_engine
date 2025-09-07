#pragma once
#include "../Program.h"

class Framebuffer;

class PostProcessProgram : public Program
{
    
public:
    std::unique_ptr<Framebuffer> framebuffer_;
    PostProcessProgram(const std::vector<Shader>&, int, int);
    void draw(Mesh&, int quad_index, const CameraComponent&, int texture_handle = -1);
    void resize(int, int);
    void bind_framebuffer();
    void unbind_framebuffer();
};
