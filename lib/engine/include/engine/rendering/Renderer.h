#pragma once

#include <glad.h>
#include <glfw3.h>

#include "../world/Scene.h"
#include "../world/Entity.h"
#include "GBuffer.h"
#include "engine/rendering/Framebuffer.h"

#include <deque>
#include <memory>


class GeometryProgram;
class LightingProgram;
class SkyboxProgram;
class PostProcessProgram;
class ShadowDepthProgram;

class ShadowMap;

class Mesh;
class TextureManager;

class Mouse;


namespace engine
{
    class Window;

    class Renderer
    {
        Window* window_;

        GeometryProgram* geometry_program_;
        LightingProgram* lighting_program_;
        SkyboxProgram* skybox_program_;
        PostProcessProgram* post_process_program_;
        ShadowDepthProgram* shadow_depth_program_;

        std::unique_ptr<GBuffer> g_buffer_;

        ShadowMap* shadow_map_;

        std::unique_ptr<Mesh> screen_mesh_;

        bool camera_toggle_;
        Mouse* mouse_;
        GLint* viewport_;

        TextureManager* textures_;

        double last_time_;
        int frame_count_;
        int last_uptime_;
        std::deque<int> fps_history_;

        bool is_fullscreen_;
        bool fullscreen_toggle_;
        bool can_escape_;

        bool render_to_fbo_;
        std::unique_ptr<Framebuffer> render_fbo_;

        void update_delta_time();
        void resize(int, int);
    public:
        double delta_time;
        int display_frame_count;
        int max_fps_plot;
        float fps_plot[30];

        Renderer(Window& window, bool render_to_fbo = false);
        ~Renderer() = default;

        void update();
        void render(Scene& scene);
        void swap_and_poll();

        Framebuffer* get_render_fbo();
    };
}
