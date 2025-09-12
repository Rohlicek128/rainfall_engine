#pragma once
#include "Framebuffer.h"
#include "../IBindable.h"

class GBuffer : public IBindable, public IGui
{
    int selected_texture_;
    std::unique_ptr<Framebuffer> framebuffer_;
public:
    Texture* position_texture;
    Texture* albedo_rough_texture;
    Texture* normal_metal_texture;
    
    GBuffer(int width, int height);
    ~GBuffer() override = default;
    void resize(int, int);
    Framebuffer& get_framebuffer();
    void blit_framebuffer();
    void active_bind();
    void bind() override;
    void unbind() override;

    void set_gui() override;
};
