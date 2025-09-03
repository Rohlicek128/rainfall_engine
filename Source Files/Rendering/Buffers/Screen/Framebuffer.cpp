#include "Framebuffer.h"

#include <iostream>

#include "glad/glad.h"

Framebuffer::Framebuffer()
{
    glGenFramebuffers(1, &handle_);
    attached_renderbuffer = nullptr;
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &handle_);
}

void Framebuffer::attach_texture_2d(std::unique_ptr<Texture> texture, const GLenum attachment)
{
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->get_handle(), 0);
    texture->attachment = attachment;
    attached_textures.push_back(std::move(texture));
    unbind();
}

void Framebuffer::attach_renderbuffer(std::unique_ptr<Renderbuffer> renderbuffer, const GLenum attachment)
{
    bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer->get_handle());
    attached_renderbuffer = std::move(renderbuffer);
    unbind();
}

bool Framebuffer::check_completeness()
{
    bind();
    const bool result = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    if(!result) std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
    unbind();
    
    return result;
}

void Framebuffer::set_draw_buffers()
{
    bind();
    unsigned int* attachments = new unsigned int[attached_textures.size()];
    for (int i = 0; i < attached_textures.size(); ++i)
    {
        attachments[i] = attached_textures.at(i)->attachment;
    }
    glDrawBuffers(attached_textures.size(), attachments);
    delete[] attachments;
    unbind();
}

void Framebuffer::resize(const int w, const int h)
{
    for (const auto& texture : attached_textures) texture->resize(w, h);
    
    if (attached_renderbuffer == nullptr || attached_renderbuffer->get_width() == w || attached_renderbuffer->get_height() == h) return;
    attached_renderbuffer->resize(w, h);
    attach_renderbuffer(std::make_unique<Renderbuffer>(*attached_renderbuffer), GL_DEPTH_STENCIL_ATTACHMENT);
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
    resize((int)window_size.x, (int)window_size.y);
        
    ImGui::Image((ImTextureID)(intptr_t)attached_textures.at(0)->get_handle(),
        {window_size.x, window_size.y},{0, 1}, {1, 0});
}
