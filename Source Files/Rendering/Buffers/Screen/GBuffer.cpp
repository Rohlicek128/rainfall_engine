#include "GBuffer.h"

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

void GBuffer::set_gui()
{
    //List
    if (ImGui::BeginChild("##GComponents", {250, 0}, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX))
    {
        if (ImGui::Selectable("Position", selected_texture_ == 0))
            selected_texture_ = 0;
        if (ImGui::Selectable("Albedo", selected_texture_ == 1))
            selected_texture_ = 1;
        if (ImGui::Selectable("Normal", selected_texture_ == 2))
            selected_texture_ = 2;

        if (ImGui::Selectable("Depth", selected_texture_ == 3))
            selected_texture_ = 3;
        
        ImGui::EndChild();
    }

    ImGui::SameLine();

    //Desc
    ImGui::BeginGroup();
    if (selected_texture_ != -1 && ImGui::BeginChild("##GDesc"))
    {
        if (selected_texture_ == 3)
        {
            Renderbuffer& renderbuffer = *framebuffer_->attached_renderbuffer;

            ImVec2 avail_size = ImGui::GetContentRegionAvail();
            ImGui::Image((ImTextureID)(intptr_t)renderbuffer.get_handle(),
            {avail_size.x, (float)renderbuffer.get_height() / (float)renderbuffer.get_width() * avail_size.x},
            {0, 1}, {1, 0});
        }
        else
        {
            Texture* texture = framebuffer_->attached_textures.at(selected_texture_).get();

            ImVec2 avail_size = ImGui::GetContentRegionAvail();
            ImGui::Image((ImTextureID)(intptr_t)texture->get_handle(),
            {avail_size.x, (float)texture->get_height() / (float)texture->get_width() * avail_size.x},
            {0, 1}, {1, 0});
        }
        ImGui::EndChild();
    }
    ImGui::EndGroup();
}
