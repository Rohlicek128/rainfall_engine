#include "engine/rendering/Framebuffer.h"

#include <iostream>

#include <glad.h>

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

void Framebuffer::attach_renderbuffer(std::unique_ptr<Renderbuffer> renderbuffer)
{
    bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, renderbuffer->get_attachment(), GL_RENDERBUFFER, renderbuffer->get_handle());
    attached_renderbuffer = std::move(renderbuffer);
    unbind();
}

bool Framebuffer::check_completeness()
{
    bind();
    const bool result = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    if(!result)
        std::cout << "ERROR::FRAMEBUFFER::H" << handle_ << " Framebuffer is not complete!\n";
    unbind();

    return result;
}

void Framebuffer::blit_framebuffer()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, handle_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, attached_textures.front()->get_width(), attached_textures.front()->get_height(),
        0, 0, attached_textures.front()->get_width(), attached_textures.front()->get_height(),
        GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    for (const auto& texture : attached_textures)
    {
        texture->resize(w, h);
        bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, texture->attachment, GL_TEXTURE_2D, texture->get_handle(), 0);
        unbind();
    }
    check_completeness();

    if (attached_renderbuffer == nullptr || (attached_renderbuffer->get_width() == w && attached_renderbuffer->get_height() == h)) return;
    attached_renderbuffer->resize(w, h);
    bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attached_renderbuffer->get_attachment(), GL_RENDERBUFFER, attached_renderbuffer->get_handle());
    unbind();
    check_completeness();
}

void Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, handle_);
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
