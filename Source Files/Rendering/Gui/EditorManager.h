#pragma once

#include "../../Imgui/imgui.h"
#include "../../Imgui/imgui_impl_glfw.h"
#include "../../Imgui/imgui_impl_opengl3.h"

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

    Entity* camera;
    ImGuiIO* imgui_io;

    Entity* selected_entity;
    
    EditorManager(bool);
    ~EditorManager() = default;
    void init_imgui(GLFWwindow*);
    void set_main_dockspace();
    
    void set_scene_graph(Scene*);
    void set_entity_inspector();
    void set_graph_children(const std::vector<Entity*>&, Entity*&);
    
    void new_frame();
    void render();
};
