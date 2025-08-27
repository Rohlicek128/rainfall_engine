#pragma once

#include "../../Imgui/imgui.h"
#include "../../Imgui/imgui_impl_glfw.h"
#include "../../Imgui/imgui_impl_opengl3.h"
#include "../Buffers/Screen/Framebuffer.h"

#include "../Entities/Entity.h"
#include "../World/Mesh.h"
#include "../World/Scene.h"

class EditorManager
{
public:
    bool is_visible;
    bool toggle_visibility;
    bool show_imgui_demo;
    bool show_statistics;
    
    ImGuiIO* imgui_io;

    std::unique_ptr<Framebuffer> viewport_fbo;
    
    EditorManager(bool, int, int);
    ~EditorManager() = default;
    void init_imgui(GLFWwindow*);
    void set_main_dockspace();
    
    void new_frame();
    void render();
};
