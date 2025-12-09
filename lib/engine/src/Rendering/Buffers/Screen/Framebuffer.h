#pragma once
#include <memory>
#include <vector>

#include "Renderbuffer.h"
#include "../IBindable.h"
#include "engine/core/IGui.h"
#include "../Textures/Texture.h"

class Framebuffer : public IBindable, public IGui
{
public:
    std::vector<std::unique_ptr<Texture>> attached_textures;
    std::unique_ptr<Renderbuffer> attached_renderbuffer;

    Framebuffer();
    ~Framebuffer() override;
    void attach_texture_2d(std::unique_ptr<Texture>, GLenum);
    void attach_renderbuffer(std::unique_ptr<Renderbuffer>);
    bool check_completeness();

    void blit_framebuffer();

    void set_draw_buffers();
    void resize(int, int);
    void bind() override;
    void unbind() override;
    void set_gui() override;
};
