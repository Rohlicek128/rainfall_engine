#include "engine/rendering/GBuffer.h"

GBuffer::GBuffer(int width, int height)
{
    framebuffer_ = std::make_unique<Framebuffer>();

    //Position
    framebuffer_->attach_texture_2d(std::make_unique<Texture>(width, height, GL_RGB16F, GL_RGB, GL_FLOAT), GL_COLOR_ATTACHMENT0);
    position_texture = framebuffer_->attached_textures.back().get();

    //Albedo + Roughness
    framebuffer_->attach_texture_2d(std::make_unique<Texture>(width, height, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE), GL_COLOR_ATTACHMENT1);
    albedo_rough_texture = framebuffer_->attached_textures.back().get();

    //Normal + Metal
    framebuffer_->attach_texture_2d(std::make_unique<Texture>(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT), GL_COLOR_ATTACHMENT2);
    normal_metal_texture = framebuffer_->attached_textures.back().get();

    framebuffer_->set_draw_buffers();

    //Renderbuffer
    framebuffer_->attach_renderbuffer(std::make_unique<Renderbuffer>(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT));
    framebuffer_->check_completeness();

    selected_texture_ = -1;
}

void GBuffer::resize(const int width, const int height)
{
    framebuffer_->resize(width, height);
}

Framebuffer& GBuffer::get_framebuffer()
{
    return *framebuffer_;
}

void GBuffer::blit_framebuffer()
{
    framebuffer_->blit_framebuffer();
}

void GBuffer::active_bind()
{
    position_texture->active_bind(0);
    albedo_rough_texture->active_bind(1);
    normal_metal_texture->active_bind(2);
}

void GBuffer::bind()
{
    framebuffer_->bind();
}

void GBuffer::unbind()
{
    framebuffer_->unbind();
}
