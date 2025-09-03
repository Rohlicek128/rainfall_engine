#pragma once
#include "Framebuffer.h"
#include "../IBindable.h"

class GBuffer : public IBindable
{
    std::unique_ptr<Framebuffer> framebuffer_;
    
public:
    Texture* position_texture;
    Texture* albedo_spec_texture;
    Texture* normal_shin_texture;
    
    GBuffer(int width, int height);
    ~GBuffer() override = default;
    void resize(int, int);
    void active_bind();
    void bind() override;
    void unbind() override;
};
