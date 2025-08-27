#pragma once
#include <queue>

#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include "EngineArgs.h"
#include "Buffers/Screen/Framebuffer.h"
#include "Buffers/Textures/TextureManager.h"
#include "Shaders/Program.h"

#include "Entities/Mouse.h"
#include "Gui/EditorManager.h"
#include "World/Scene.h"

class Engine
{
    std::unique_ptr<Scene> scene_;
    Program* program_;

    Program* screen_program_;
    Mesh* screen_mesh_;
    std::shared_ptr<Texture> screen_texture_;
    std::unique_ptr<Framebuffer> screen_fbo_;
    std::shared_ptr<Renderbuffer> screen_rbo_;
    
    Entity* current_camera_;
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
    float fps_plot_[60];
    std::deque<int> fps_history_;
    
    bool is_fullscreen_;
    bool fullscreen_toggle_;
    bool can_escape_;
    
    Entity* entity_selected_;

    void update_delta_time();
    void set_icon(GLFWwindow*, const std::string&);
    void set_models_to_mesh(Mesh*);
public:
    Engine(const EngineArgs&);
    ~Engine();
    
    void update(const EngineArgs&);
    void render(const EngineArgs&);
};
