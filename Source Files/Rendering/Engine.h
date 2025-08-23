#pragma once
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_glfw.h"
#include "../Imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include "EngineArgs.h"
#include "Buffers/Textures/TextureManager.h"
#include "Shaders/Program.h"

#include "Entities/Mouse.h"
#include "Gui/EditorManager.h"

class Engine
{
    Program* program_;
    Mesh* mesh_;
    std::vector<std::shared_ptr<Entity>> lights_;

    std::shared_ptr<Entity> player_camera_;
    Entity* current_camera_;
    Mouse* mouse_;
    GLint* viewport_;

    TextureManager* textures_;
    EditorManager* editor_;

    double delta_time_;
    double last_time_;

    bool is_wireframe_;
    bool is_fullscreen_;
    bool fullscreen_toggle_;
    bool can_escape_;
    
    int entity_select_;

    void update_delta_time();
public:
    Engine(const EngineArgs&);
    ~Engine();
    
    void update(const EngineArgs&);
    void render(const EngineArgs&);
};
