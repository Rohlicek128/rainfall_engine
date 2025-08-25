#pragma once

#include "../../Imgui/imgui.h"
#include "../../Imgui/imgui_impl_glfw.h"
#include "../../Imgui/imgui_impl_opengl3.h"

#include "../Entities/Entity.h"
#include "../Buffers/Mesh.h"

class EditorManager
{
public:
    bool is_visible;
    bool toggle_visibility;
    bool show_imgui_demo;
    bool show_statistics;

    Entity* camera;
    ImGuiIO* imgui_io;
    
    EditorManager(bool);
    ~EditorManager() = default;
    void init_imgui(GLFWwindow*);
    void set_main_dockspace();
    void set_graph_children(const std::vector<std::shared_ptr<Entity>>&, int*, int*, bool = true);
    void new_frame();
    void render();
};
