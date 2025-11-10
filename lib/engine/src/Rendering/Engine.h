#pragma once
#include <queue>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include "EngineArgs.h"
#include "Buffers/Screen/GBuffer.h"
#include "Buffers/Textures/TextureManager.h"

#include "Entities/Mouse.h"
#include "Gui/EditorManager.h"
#include "Shaders/Programs/GeometryProgram.h"
#include "Shaders/Programs/LightingProgram.h"
#include "Shaders/Programs/PostProcessProgram.h"
#include "Shaders/Programs/SkyboxProgram.h"
#include "../Organization/Project.h"
#include "Shaders/Programs/ShadowDepthProgram.h"
#include "Shaders/Shadows/ShadowMap.h"

class Engine
{
    std::unique_ptr<Project> project_;
    Scene* current_scene_;

    std::unique_ptr<GBuffer> g_buffer_;

    GeometryProgram* geometry_program_;
    LightingProgram* lighting_program_;
    SkyboxProgram* skybox_program_;
    PostProcessProgram* post_process_program_;
    ShadowDepthProgram* shadow_depth_program_;

    ShadowMap* shadow_map_;

    std::unique_ptr<Mesh> screen_mesh_;

    bool camera_toggle_;
    Mouse* mouse_;
    GLint* viewport_;

    TextureManager* textures_;
    EditorManager* editor_;

    double delta_time_;
    double last_time_;

    int frame_count_;
    int display_frame_count_;
    int last_uptime_;
    int max_fps_plot_;
    float fps_plot_[30];
    std::deque<int> fps_history_;

    bool is_fullscreen_;
    bool fullscreen_toggle_;
    bool can_escape_;

    void update_delta_time();
    void resize(int, int);
    void set_icon(GLFWwindow*, const std::string&);

    void set_models_to_mesh(Mesh*);
    void set_hardcoded_entities(Scene&);
public:
    Engine(const EngineArgs&);
    ~Engine();

    void update(const EngineArgs&);
    void render(EngineArgs&);
};
