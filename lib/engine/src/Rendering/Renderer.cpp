#include "engine/rendering/Renderer.h"

#include <glad.h>

#include "engine/rendering/Window.h"
#include "EngineArgs.h"
#include "engine/world/Scene.h"

#include "Buffers/Screen/GBuffer.h"
#include "Buffers/Textures/TextureManager.h"

#include "Entities/Mouse.h"
#include "Shaders/Programs/GeometryProgram.h"
#include "Shaders/Programs/LightingProgram.h"
#include "Shaders/Programs/PostProcessProgram.h"
#include "Shaders/Programs/SkyboxProgram.h"
#include "Shaders/Programs/ShadowDepthProgram.h"
#include "Shaders/Shadows/ShadowMap.h"

#include "engine/world/Components/CameraComponent.h"
#include "engine/world/Components/LightComponent.h"

#include "engine/managers/Mesh.h"

#include "engine/world/Entity.h"


namespace engine
{
    Renderer::Renderer(Window& window)
    {
        window_ = &window;

        textures_ = TextureManager::get_instance();
        textures_->add_essential_texture(std::make_unique<Texture>("engine/assets/white1x1.png", GL_RGBA8, GL_RGBA));
        textures_->add_essential_texture(std::make_unique<Texture>("engine/assets/black1x1.png", GL_RGBA8, GL_RGBA));
        textures_->add_essential_texture(std::make_unique<Texture>("engine/assets/missing_texture.png", GL_SRGB8_ALPHA8, GL_RGBA));

        //std::vector<std::string> faces = {"assets/Skybox/0right.jpg", "assets/Skybox/1left.jpg", "assets/Skybox/2top.jpg",
        //    "assets/Skybox/3bottom.jpg", "assets/Skybox/4front.jpg", "assets/Skybox/5back.jpg"};
        //textures_->add_cubemap(std::make_unique<Cubemap>(faces, GL_SRGB8, GL_RGB, GL_UNSIGNED_BYTE));


        viewport_ = new GLint[2]{window_->engine_args.width, window_->engine_args.height};
        mouse_ = new Mouse(viewport_[0], viewport_[1], 5.0f, false);
        if (mouse_->is_visible)
        {
            glfwSetInputMode(window_->engine_args.window, GLFW_CURSOR,  GLFW_CURSOR_NORMAL);
            //editor_->imgui_io->ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        }
        else
        {
            glfwSetInputMode(window_->engine_args.window, GLFW_CURSOR,  GLFW_CURSOR_DISABLED);
            //editor_->imgui_io->ConfigFlags |= ImGuiConfigFlags_NoMouse;
        }


        g_buffer_ = std::make_unique<GBuffer>(window_->engine_args.width, window_->engine_args.height);
        geometry_program_ = new GeometryProgram({{"Geometry/geometry.vert", GL_VERTEX_SHADER}, {"Geometry/geometry.frag", GL_FRAGMENT_SHADER}});
        lighting_program_ = new LightingProgram({{"Screen/screen.vert", GL_VERTEX_SHADER}, {"Lighting/pbr_lighting.frag", GL_FRAGMENT_SHADER}});


        //Screen Mesh
        float vertices_quad[] = {
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
        };
        unsigned int indices_quad[] = {
            0, 1, 2,
            0, 2, 3
        };
        screen_mesh_ = std::make_unique<Mesh>(std::vector<VertexAttribute>{{"PosTex", 4}});
        screen_mesh_->add_model("Quad", vertices_quad, std::size(vertices_quad), indices_quad, std::size(indices_quad));
        screen_mesh_->compile();

        post_process_program_ = new PostProcessProgram(
            {{"Screen/screen.vert", GL_VERTEX_SHADER}, {"Screen/screen.frag", GL_FRAGMENT_SHADER}},
            window_->engine_args.width, window_->engine_args.height
        );

        skybox_program_ = new SkyboxProgram({{"Skybox/skybox.vert", GL_VERTEX_SHADER}, {"Skybox/skybox.frag", GL_FRAGMENT_SHADER}});

        shadow_depth_program_ = new ShadowDepthProgram(
            {{"ShadowDepth/shadow_depth.vert", GL_VERTEX_SHADER},
            {"ShadowDepth/shadow_depth.frag", GL_FRAGMENT_SHADER}});
        shadow_map_ = new ShadowMap(2048, 2048, 32.0f, -5.0f, 45.0f);

        is_fullscreen_ = false;
        fullscreen_toggle_ = true;
        camera_toggle_ = true;
        can_escape_ = true;

        delta_time = 0.0f;
        last_time_ = 0.0;

        display_frame_count_ = 0;
        frame_count_ = 0;
        last_uptime_ = 0;
        max_fps_plot_ = 0;

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_CCW);

        glEnable(GL_PROGRAM_POINT_SIZE);
        glPointSize(5.0f);
    }

    void Renderer::update_delta_time()
    {
        const double current = glfwGetTime();
        delta_time = current - last_time_;
        last_time_ = current;

        if ((int)floor(current) > last_uptime_)
        {
            last_uptime_ = (int)floor(current);
            display_frame_count_ = frame_count_;
            frame_count_ = 0;
            max_fps_plot_ = 0;

            fps_history_.push_back(display_frame_count_);
            if (fps_history_.size() > std::size(fps_plot_)) fps_history_.pop_front();
            for (unsigned int i = 0; i < fps_history_.size(); ++i)
            {
                fps_plot_[i] = (float)fps_history_.at(i);
                max_fps_plot_ = std::max<int>(fps_history_.at(i), max_fps_plot_);
            }
        }
        frame_count_++;
    }


    void Renderer::update()
    {
        update_delta_time();
        mouse_->pos_x = window_->engine_args.mouse_x;
        mouse_->pos_y = window_->engine_args.mouse_y;

        if (viewport_[0] != window_->engine_args.width || viewport_[1] != window_->engine_args.height)
            resize(window_->engine_args.width, window_->engine_args.height);
        viewport_[0] = window_->engine_args.width;
        viewport_[1] = window_->engine_args.height;


        //END
        if (glfwGetKey(window_->engine_args.window, GLFW_KEY_END) == GLFW_PRESS) glfwSetWindowShouldClose(window_->engine_args.window, true);

        //ESCAPE
        if (glfwGetKey(window_->engine_args.window, GLFW_KEY_ESCAPE) == GLFW_PRESS && can_escape_)
        {
            can_escape_ = false;
            mouse_->is_visible = !mouse_->is_visible;
            mouse_->first_move = true;
            if (mouse_->is_visible)
            {
                glfwSetInputMode(window_->engine_args.window, GLFW_CURSOR,  GLFW_CURSOR_NORMAL);
            }
            else
            {
                glfwSetInputMode(window_->engine_args.window, GLFW_CURSOR,  GLFW_CURSOR_DISABLED);
            }
        }
        else if (!can_escape_ && glfwGetKey(window_->engine_args.window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) can_escape_ = true;

        //F11
        if (glfwGetKey(window_->engine_args.window, GLFW_KEY_F11) == GLFW_PRESS && fullscreen_toggle_)
        {
            fullscreen_toggle_ = false;
            is_fullscreen_ = !is_fullscreen_;
            if (is_fullscreen_) glfwSetWindowMonitor(window_->engine_args.window, glfwGetPrimaryMonitor(), 50, 50, window_->engine_args.width, window_->engine_args.height, GLFW_DONT_CARE);
            else glfwSetWindowMonitor(window_->engine_args.window, nullptr, 50, 50, window_->engine_args.width, window_->engine_args.height, GLFW_DONT_CARE);
        }
        else if (!fullscreen_toggle_ && glfwGetKey(window_->engine_args.window, GLFW_KEY_F11) == GLFW_RELEASE) fullscreen_toggle_ = true;
    }

    void Renderer::render(Scene& scene)
    {
        CameraComponent* cur_camera_comp = scene.current_camera->get_component<CameraComponent>();
        cur_camera_comp->move(window_->engine_args.window, static_cast<float>(delta_time));
        if (!mouse_->is_visible) cur_camera_comp->mouse_move(*mouse_, static_cast<float>(delta_time));

        const int cur_width = window_->engine_args.width;
        const int cur_height = window_->engine_args.height;

        //Shadows
        const std::vector<Entity*> dir_lights = scene.get_lights_by_type(lights::LIGHT_TYPE::DIRECTIONAL);
        if (shadow_map_->is_visible && !dir_lights.empty())
        {
            shadow_map_->render_depth_map(*dir_lights.front(), scene, *shadow_depth_program_);
        }

        //Geometry Pass
        g_buffer_->bind();
        glViewport(0, 0, cur_width, cur_height);
        glClearColor(cur_camera_comp->clear_color[0], cur_camera_comp->clear_color[1], cur_camera_comp->clear_color[2], cur_camera_comp->clear_color[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (cur_camera_comp->is_wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        geometry_program_->draw(scene, (float)cur_width / (float)cur_height);

        if (cur_camera_comp->is_wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        g_buffer_->unbind();

        //Lighting Pass
        post_process_program_->bind_framebuffer();
        //glClearColor(cur_camera_comp->clear_color[0], cur_camera_comp->clear_color[1], cur_camera_comp->clear_color[2], cur_camera_comp->clear_color[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        lighting_program_->draw(scene, *screen_mesh_, 0, *g_buffer_, *shadow_map_);
        post_process_program_->unbind_framebuffer();


        //Post Process Pass
        post_process_program_->draw(*screen_mesh_, 0, *cur_camera_comp, shadow_map_->is_debug_visible ? shadow_map_->get_depth_map()->attached_textures.at(0)->get_handle() : -1);


        glViewport(0, 0, window_->engine_args.width, window_->engine_args.height);
    }

    void Renderer::swap_and_poll()
    {
        window_->engine_args.scroll_x = 0.0f;
        window_->engine_args.scroll_y = 0.0f;

        glfwSwapBuffers(window_->engine_args.window);
        glfwPollEvents();
    }

    void Renderer::resize(const int width, const int height)
    {
        g_buffer_->resize(width, height);
        post_process_program_->resize(width, height);
    }
}
