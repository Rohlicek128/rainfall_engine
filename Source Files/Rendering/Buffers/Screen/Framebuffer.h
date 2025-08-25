#pragma once
#include <memory>
#include <vector>

#include "Renderbuffer.h"
#include "../IBindable.h"
#include "../../Gui/IGui.h"
#include "../Textures/Texture.h"

class Framebuffer : public IBindable, public IGui
{
public:
    std::vector<std::shared_ptr<Texture>> attached_textures;
    std::shared_ptr<Renderbuffer> attached_renderbuffer;
    
    Framebuffer();
    ~Framebuffer() override;
    void attach_texture_2d(const std::shared_ptr<Texture>&, GLenum);
    void attach_renderbuffer(const std::shared_ptr<Renderbuffer>&, GLenum);
    bool check_completeness();
    void resize(int, int);
    void bind() override;
    void unbind() override;
    void set_gui() override;
};
