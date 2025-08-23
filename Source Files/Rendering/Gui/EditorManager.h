#pragma once
#include <memory>

#include "../../Imgui/imgui.h"
#include "../../Imgui/imgui_impl_glfw.h"
#include "../../Imgui/imgui_impl_opengl3.h"

#include "../Entities/Entity.h"

class EditorManager
{
public:
    bool is_visible;
    bool toggle_visibility;

    Entity* camera;
    ImGuiIO* imgui_io;
    
    EditorManager(bool);
    ~EditorManager() = default;
    void init_imgui(GLFWwindow*);
    void new_frame();
    void render();
};
