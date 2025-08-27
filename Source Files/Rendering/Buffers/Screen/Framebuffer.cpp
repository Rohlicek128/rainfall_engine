#include "Framebuffer.h"

#include <iostream>

#include "glad/glad.h"

Framebuffer::Framebuffer()
{
    glGenFramebuffers(1, &handle_);
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &handle_);
}

void Framebuffer::attach_texture_2d(const std::shared_ptr<Texture>& texture, const GLenum attachment)
{
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->get_handle(), 0);
    glDrawBuffers(1, &attachment);
    glReadBuffer(attachment);
    attached_textures.push_back(texture);
    unbind();
}

void Framebuffer::attach_renderbuffer(const std::shared_ptr<Renderbuffer>& renderbuffer, const GLenum attachment)
{
    bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer->get_handle());
    attached_renderbuffer = renderbuffer;
    unbind();
}

bool Framebuffer::check_completeness()
{
    Framebuffer::bind();
    const bool result = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    if(!result) std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
    Framebuffer::unbind();
    
    return result;
}

void Framebuffer::resize(const int w, const int h)
{
    for (const auto& texture : attached_textures) texture->resize(w, h);
}

void Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, handle_);
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::set_gui()
{
    if (attached_textures.at(0) == nullptr) return;
    
    ImVec2 window_size = ImGui::GetWindowContentRegionMax();
    window_size.x -= ImGui::GetWindowContentRegionMin().x;
    window_size.y -= ImGui::GetWindowContentRegionMin().y;
    if (attached_textures.at(0)->get_width() != (int)window_size.x || attached_textures.at(0)->get_height() != (int)window_size.y)
    {
        resize((int)window_size.x, (int)window_size.y);
        if (attached_renderbuffer != nullptr)
        {
            attached_renderbuffer->resize((int)window_size.x, (int)window_size.y);
            attach_renderbuffer(attached_renderbuffer, GL_DEPTH_STENCIL_ATTACHMENT);
        }
    }
        
    ImGui::Image((ImTextureID)(intptr_t)attached_textures.at(0)->get_handle(),
        {window_size.x, window_size.y},{0, 1}, {1, 0});
}
