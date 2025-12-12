#pragma once
#include <memory>
#include <glm/mat4x4.hpp>

#include "engine/core/IGui.h"

class Entity;
class Scene;
class ShadowDepthProgram;
class Framebuffer;

class ShadowMap : public IGui
{
    int width_, height_;
    float near_plane_, far_plane_, zoom_;
    std::unique_ptr<Framebuffer> depth_map_;
public:
    bool is_visible;
    bool is_debug_visible;
    glm::mat4 light_space;

    ShadowMap(int width, int height, float zoom, float near, float far);
    ~ShadowMap() override = default;

    void render_depth_map(Entity& light, const Scene& scene, ShadowDepthProgram& program);
    Framebuffer* get_depth_map();

    void set_gui() override;
};
