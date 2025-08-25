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

class Engine
{
    Program* program_;
    Mesh* mesh_;
    std::vector<std::shared_ptr<Entity>> lights_;

    Program* screen_program;
    Mesh* screen_mesh_;
    std::shared_ptr<Texture> screen_texture_;
    std::unique_ptr<Framebuffer> screen_fbo_;
    std::shared_ptr<Renderbuffer> screen_rbo_;

    std::shared_ptr<Entity> player_camera_;
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
    float fps_plot_[20];
    std::deque<int> fps_history_;
    
    bool is_fullscreen_;
    bool fullscreen_toggle_;
    bool can_escape_;
    
    int entity_select_;

    void update_delta_time();
    void set_icon(GLFWwindow*, const std::string&);
public:
    Engine(const EngineArgs&);
    ~Engine();
    
    void update(const EngineArgs&);
    void render(const EngineArgs&);
};
