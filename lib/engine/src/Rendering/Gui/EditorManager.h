#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../Buffers/Screen/Framebuffer.h"

class Mesh;
class Project;
#include "../EngineArgs.h"

class EditorManager
{
public:
    bool is_visible;
    bool toggle_visibility;
    bool show_imgui_demo;
    bool show_statistics;
    bool show_g_buffer_inspector;

    int gizmo_operation;

    ImGuiIO* imgui_io;

    std::unique_ptr<Framebuffer> viewport_fbo;

    EditorManager(bool, int, int);
    ~EditorManager() = default;
    void init_imgui(GLFWwindow*);
    void set_main_dockspace(const EngineArgs&, Project&);

    void set_red_style();

    void new_frame();
    void render();
};
