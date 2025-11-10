#include "ShadowMap.h"

#include "../../Buffers/Screen/Framebuffer.h"
#include "../../Entities/Entity.h"
#include "../../Entities/Components/LightComponent.h"
#include "../Programs/ShadowDepthProgram.h"

ShadowMap::ShadowMap(const int width, const int height, const float zoom, const float near, const float far)
{
    width_ = width;
    height_ = height;
    near_plane_ = near;
    far_plane_ = far;
    zoom_ = zoom;
    is_visible = true;
    is_debug_visible = false;

    depth_map_ = std::make_unique<Framebuffer>();
    depth_map_->attach_texture_2d(std::make_unique<Texture>(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT), GL_DEPTH_ATTACHMENT);

    depth_map_->attached_textures.at(0)->bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    const float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
    depth_map_->attached_textures.at(0)->unbind();

    depth_map_->bind();
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    depth_map_->unbind();
}

void ShadowMap::render_depth_map(Entity& light, const Scene& scene, ShadowDepthProgram& program)
{
    LightComponent* light_component = light.get_component<LightComponent>();
    if (!is_visible || !light_component || !light_component->is_enabled || light_component->type != lights::LIGHT_TYPE::DIRECTIONAL) return;

    glViewport(0, 0, width_, height_);
    depth_map_->bind();

    glClear(GL_DEPTH_BUFFER_BIT);
    light_space = light_component->get_light_space_mat(
        light.transform->rotation, scene.current_camera->transform->position,
        zoom_, near_plane_, far_plane_);
    program.draw(scene, light_space);

    depth_map_->unbind();
}

Framebuffer* ShadowMap::get_depth_map()
{
    return depth_map_.get();
}

void ShadowMap::set_gui()
{
    ImGui::Checkbox("Visible", &is_visible);
    ImGui::Checkbox("Debug", &is_debug_visible);
    ImGui::Text("Width: %i, Height: %i", width_, height_);
    ImGui::DragFloat("Zoom", &zoom_, 0.1f, 0, 0, "%.1f");
    ImGui::DragFloat("Near Plane", &near_plane_, 0.01f, 0, 0, "%.2f");
    ImGui::DragFloat("Far Plane", &far_plane_, 0.01f, 0, 0, "%.2f");
}
