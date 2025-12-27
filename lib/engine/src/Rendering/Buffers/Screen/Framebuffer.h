#pragma once
#include <memory>
#include <vector>

#include "Renderbuffer.h"
#include "engine/core/IBindable.h"
#include "engine/rendering/Texture.h"

class Framebuffer : public IBindable
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
};
