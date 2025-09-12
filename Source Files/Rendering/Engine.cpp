#include "Engine.h"

#include <algorithm>
#include <iostream>

#include "../Imgui/ImGuizmo.h"
#include "Entities/Components/CameraComponent.h"
#include "Entities/Components/LightComponent.h"
#include "Entities/Components/MaterialComponent.h"
#include "Entities/Components/MeshComponent.h"
#include "Entities/Components/TextureComponent.h"

#include "Shaders/Shader.h"

Engine::Engine(const EngineArgs& args) : fps_plot_{}
{
    set_icon(args.window, "Icon/green_tick.png");

    textures_ = TextureManager::get_instance();
    textures_->add_texture(std::make_unique<Texture>("white1x1.png", GL_RGBA8, GL_RGBA));
    textures_->add_texture(std::make_unique<Texture>("black1x1.png", GL_RGBA8, GL_RGBA));
    textures_->add_texture(std::make_unique<Texture>("missing_texture.png", GL_SRGB8_ALPHA8, GL_RGBA));
    textures_->add_texture(std::make_unique<Texture>("chill_guy.jpg", GL_SRGB8, GL_RGB));
    textures_->add_texture(std::make_unique<Texture>("container_diffuse.png", GL_SRGB8_ALPHA8, GL_RGBA));
    textures_->add_texture(std::make_unique<Texture>("container_specular.png", GL_RGBA8, GL_RGBA));
    textures_->add_texture(std::make_unique<Texture>("circuits_normal.jpg", GL_RGB8, GL_RGB));
    textures_->add_texture(std::make_unique<Texture>("shrek.png", GL_SRGB8_ALPHA8, GL_RGBA));
    textures_->add_texture(std::make_unique<Texture>("black_hole.jpg", GL_SRGB8, GL_RGB));

    textures_->add_texture(std::make_unique<Texture>("rocky_dirt/rocky_trail_02_diff_1k.jpg", GL_SRGB8, GL_RGB));
    textures_->add_texture(std::make_unique<Texture>("rocky_dirt/rocky_trail_02_arm_1k.jpg", GL_RGB8, GL_RGB));
    textures_->add_texture(std::make_unique<Texture>("rocky_dirt/rocky_trail_02_nor_gl_1k.jpg", GL_RGB8, GL_RGB));

    std::vector<std::string> faces = {"Skybox/0right.jpg", "Skybox/1left.jpg", "Skybox/2top.jpg",
        "Skybox/3bottom.jpg", "Skybox/4front.jpg", "Skybox/5back.jpg"};
    textures_->add_cubemap(std::make_unique<Cubemap>(faces, GL_SRGB8, GL_RGB, GL_UNSIGNED_BYTE));
    

    editor_ = new EditorManager(true, args.width, args.height);
    editor_->init_imgui(args.window);

    viewport_ = new GLint[2]{args.width, args.height};
    mouse_ = new Mouse(viewport_[0], viewport_[1], 5.0f, editor_->is_visible);
    if (mouse_->is_visible)
    {
        glfwSetInputMode(args.window, GLFW_CURSOR,  GLFW_CURSOR_NORMAL);
        //editor_->imgui_io->ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }
    else
    {
        glfwSetInputMode(args.window, GLFW_CURSOR,  GLFW_CURSOR_DISABLED);
        //editor_->imgui_io->ConfigFlags |= ImGuiConfigFlags_NoMouse;
    }

    
    //Scene
    scene_ = std::make_unique<Scene>("Test Scene", Mesh({{"Position", 3}, {"TexCoord", 2}, {"Normal", 3}}, true));

    set_models_to_mesh(scene_->mesh.get());
    scene_->mesh->compile();

    //Entities
    std::unique_ptr<Entity> player_camera = std::make_unique<Entity>("Player camera",
        new TransformComponent(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.3f))
    );
    player_camera->add_component(CAMERA, new CameraComponent(player_camera->transform));
    player_camera->add_component(MESH, new MeshComponent(1));
    scene_->player_camera = player_camera.get();

    std::unique_ptr<Entity> obj1 = std::make_unique<Entity>("Cube",
        new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f))
    );
    obj1->add_component(MESH, new MeshComponent(0));
    obj1->add_component(MATERIAL, new MaterialComponent(glm::vec4(1.0, 1.0, 1.0, 1.0)));
    obj1->add_component(TEXTURE, new TextureComponent(GL_TEXTURE_2D, textures_->get_texture(4)->get_handle(),textures_->get_texture(5)->get_handle()));

    std::unique_ptr<Entity> obj2 = std::make_unique<Entity>("Cube #2",
        new TransformComponent(glm::vec3(5.0f, 2.0f, 3.0f), glm::vec3(0.0f), glm::vec3(3.0f))
    );
    obj2->add_component(MESH, new MeshComponent(0));
    obj2->add_component(TEXTURE, new TextureComponent(GL_TEXTURE_2D, textures_->get_texture(3)->get_handle(), 1));
    //obj1->add_child(obj2.get());

    std::unique_ptr<Entity> obj3 = std::make_unique<Entity>("Paper plane #2",
        new TransformComponent(glm::vec3(-2.0f, 1.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.5f))
    );
    obj3->add_component(MATERIAL, new MaterialComponent(glm::vec4(0.0, 0.0, 1.0, 1.0)));
    obj2->add_child(obj3.get());

    std::unique_ptr<Entity> floor = std::make_unique<Entity>("Floor",
        new TransformComponent(glm::vec3(0.0f, -1.001f, 0.0f), glm::vec3(0.0f), glm::vec3(50.0f))
    );
    floor->add_component(MESH, new MeshComponent(1));
    floor->add_component(MATERIAL, new MaterialComponent(glm::vec4(0.012, 0.012, 0.012, 1.0)));

    std::unique_ptr<Entity> dir_light = std::make_unique<Entity>("Sun",
        new TransformComponent(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(-6.0f, -10, 6.0f), glm::vec3(0.5f))
    );
    dir_light->add_component(MESH, new MeshComponent(0));
    dir_light->add_component(LIGHT, new LightComponent(DIRECTIONAL, glm::vec3(1.0f), 5.0f));

    std::unique_ptr<Entity> point_light = std::make_unique<Entity>("Bulb",
        new TransformComponent(glm::vec3(3.0f, 5.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.35f))
    );
    point_light->add_component(MESH, new MeshComponent(0));
    point_light->add_component(LIGHT, new LightComponent(POINT, glm::vec3(1.0f, 1.0f, 0.0f), 0.5f));

    std::unique_ptr<Entity> point_light_2 = std::make_unique<Entity>("Bulb #2",
        new TransformComponent(glm::vec3(3.0f, 5.0f, 4.0f), glm::vec3(0.0f), glm::vec3(0.35f))
    );
    point_light_2->add_component(MESH, new MeshComponent(0));
    point_light_2->add_component(LIGHT, new LightComponent(POINT, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f));
    
    scene_->add_entity(std::move(player_camera));
    scene_->add_entity(std::move(obj1));
    scene_->add_entity(std::move(obj2));
    scene_->add_entity(std::move(obj3));
    scene_->add_entity(std::move(floor));
    
    scene_->add_entity(std::move(dir_light), true);
    scene_->add_entity(std::move(point_light), true);
    scene_->add_entity(std::move(point_light_2), true);

    g_buffer_ = std::make_unique<GBuffer>(args.width, args.height);
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
    screen_mesh_ = new Mesh({{"PosTex", 4}});
    screen_mesh_->add_model("Quad", vertices_quad, std::size(vertices_quad), indices_quad, std::size(indices_quad));
    screen_mesh_->compile();
    
    post_process_program_ = new PostProcessProgram({{"Screen/screen.vert", GL_VERTEX_SHADER}, {"Screen/screen.frag", GL_FRAGMENT_SHADER}}, args.width, args.height);
    
    skybox_program_ = new SkyboxProgram({{"Skybox/skybox.vert", GL_VERTEX_SHADER}, {"Skybox/skybox.frag", GL_FRAGMENT_SHADER}});
    

    scene_->current_camera = editor_->is_visible ? scene_->editor_camera.get() : scene_->player_camera;
    entity_selected_ = nullptr;
    
    is_fullscreen_ = false;
    fullscreen_toggle_ = true;
    camera_toggle_ = true;
    can_escape_ = true;

    delta_time_ = 0.0f;
    last_time_ = 0.0;

    display_frame_count_ = 0;
    frame_count_ = 0;
    last_uptime_ = 0;
    max_fps_plot_ = 0;

    editor_->imgui_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    editor_->imgui_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_CCW);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(5.0f);

    std::cout << "START" << '\n';
}

Engine::~Engine()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwTerminate();
    std::cout << "END" << '\n';
}


void Engine::update_delta_time()
{
    const double current = glfwGetTime();
    delta_time_ = current - last_time_;
    last_time_ = current;

    if ((int)floor(glfwGetTime()) > last_uptime_)
    {
        last_uptime_ = (int)floor(glfwGetTime());
        display_frame_count_ = frame_count_;
        frame_count_ = 0;
        max_fps_plot_ = 0;

        fps_history_.push_back(display_frame_count_);
        if (fps_history_.size() > std::size(fps_plot_)) fps_history_.pop_front();
        for (int i = 0; i < fps_history_.size(); ++i)
        {
            fps_plot_[i] = (float)fps_history_.at(i);
            max_fps_plot_ = std::max<int>(fps_history_.at(i), max_fps_plot_);
        }
    }
    frame_count_++;
}

void Engine::set_icon(GLFWwindow* window, const std::string& path)
{
    GLFWimage images[1]; 
    images[0].pixels = stbi_load(("assets/" + path).c_str(), &images[0].width, &images[0].height, nullptr, 4);
    glfwSetWindowIcon(window, 1, images); 
    stbi_image_free(images[0].pixels);
}

void Engine::update(const EngineArgs& args)
{
    update_delta_time();
    mouse_->pos_x = args.mouse_x;
    mouse_->pos_y = args.mouse_y;
    if (!editor_->is_visible && (viewport_[0] != args.width || viewport_[1] != args.height))
        resize(args.width, args.height);
    viewport_[0] = args.width;
    viewport_[1] = args.height;

    scene_->entities.at(1)->transform->rotation.y += (float)delta_time_ * 20.0f;
    scene_->entities.at(1)->transform->update_rot_edit();

    //Input
    if (glfwGetKey(args.window, GLFW_KEY_END) == GLFW_PRESS) glfwSetWindowShouldClose(args.window, true);

    //ESCAPE
    if (glfwGetKey(args.window, GLFW_KEY_ESCAPE) == GLFW_PRESS && can_escape_)
    {
        can_escape_ = false;
        mouse_->is_visible = !mouse_->is_visible;
        mouse_->first_move = true;
        if (mouse_->is_visible)
        {
            glfwSetInputMode(args.window, GLFW_CURSOR,  GLFW_CURSOR_NORMAL);
            editor_->imgui_io->ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        }
        else
        {
            glfwSetInputMode(args.window, GLFW_CURSOR,  GLFW_CURSOR_DISABLED);
            editor_->imgui_io->ConfigFlags |= ImGuiConfigFlags_NoMouse;
        }
    }
    else if (!can_escape_ && glfwGetKey(args.window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) can_escape_ = true;

    //HOME
    if (glfwGetKey(args.window, GLFW_KEY_HOME) == GLFW_PRESS && editor_->toggle_visibility)
    {
        editor_->toggle_visibility = false;
        editor_->is_visible = !editor_->is_visible;
        if (!editor_->is_visible)
            resize(args.width, args.height);
    }
    else if (!editor_->toggle_visibility && glfwGetKey(args.window, GLFW_KEY_HOME) == GLFW_RELEASE) editor_->toggle_visibility = true;

    //INS
    if (glfwGetKey(args.window, GLFW_KEY_INSERT) == GLFW_PRESS && camera_toggle_)
    {
        camera_toggle_ = false;
        if (scene_->current_camera != scene_->editor_camera.get()) scene_->current_camera = scene_->editor_camera.get();
        else if (scene_->current_camera != scene_->player_camera && scene_->player_camera != nullptr) scene_->current_camera = scene_->player_camera;
    }
    else if (!camera_toggle_ && glfwGetKey(args.window, GLFW_KEY_INSERT) == GLFW_RELEASE) camera_toggle_ = true;
    
    //F11
    if (glfwGetKey(args.window, GLFW_KEY_F11) == GLFW_PRESS && fullscreen_toggle_)
    {
        fullscreen_toggle_ = false;
        is_fullscreen_ = !is_fullscreen_;
        if (is_fullscreen_) glfwSetWindowMonitor(args.window, glfwGetPrimaryMonitor(), 50, 50, args.width, args.height, GLFW_DONT_CARE);
        else glfwSetWindowMonitor(args.window, nullptr, 50, 50, args.width, args.height, GLFW_DONT_CARE);
    }
    else if (!fullscreen_toggle_ && glfwGetKey(args.window, GLFW_KEY_F11) == GLFW_RELEASE) fullscreen_toggle_ = true;

    //Gizmo
    if (glfwGetKey(args.window, GLFW_KEY_F5)) editor_->gizmo_operation = -1;
    else if (glfwGetKey(args.window, GLFW_KEY_F6)) editor_->gizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
    else if (glfwGetKey(args.window, GLFW_KEY_F7)) editor_->gizmo_operation = ImGuizmo::OPERATION::ROTATE;
    else if (glfwGetKey(args.window, GLFW_KEY_F8)) editor_->gizmo_operation = ImGuizmo::OPERATION::SCALE;


    //Camera update
    CameraComponent* camera_component = dynamic_cast<CameraComponent*>(scene_->current_camera->components->at(CAMERA));
    camera_component->move(args.window, static_cast<float>(delta_time_));
    if (!mouse_->is_visible) camera_component->mouse_move(*mouse_, static_cast<float>(delta_time_));

    camera_component->exposure += camera_component->exposure * args.scroll_y * static_cast<float>(delta_time_) * 175.0f;
    camera_component->exposure = std::max(0.001f, camera_component->exposure);
    
    //if (editor_->imgui_io->WantCaptureMouse) std::cout << "wants\n";
}

void Engine::render(EngineArgs& args)
{
    editor_->new_frame();

    const CameraComponent* cur_camera_comp = dynamic_cast<CameraComponent*>(scene_->current_camera->components->at(CAMERA));
    const int cur_width = editor_->is_visible ? editor_->viewport_fbo->attached_textures.at(0)->get_width() : args.width;
    const int cur_height = editor_->is_visible ? editor_->viewport_fbo->attached_textures.at(0)->get_height() : args.height;

    //Geometry Pass
    g_buffer_->bind();
    glViewport(0, 0, cur_width, cur_height);
    glClearColor(cur_camera_comp->clear_color[0], cur_camera_comp->clear_color[1], cur_camera_comp->clear_color[2], cur_camera_comp->clear_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (cur_camera_comp->is_wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    geometry_program_->draw(*scene_, (float)cur_width / (float)cur_height);
    
    if (cur_camera_comp->is_wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    g_buffer_->unbind();

    //Lighting Pass
    post_process_program_->bind_framebuffer();
    lighting_program_->draw(*scene_, *screen_mesh_, 0, *g_buffer_);
    post_process_program_->unbind_framebuffer();

    g_buffer_->blit_framebuffer();
    skybox_program_->draw(*scene_, (float)cur_width / (float)cur_height);
    
    //Post Process Pass
    if (editor_->is_visible) editor_->viewport_fbo->bind();
    post_process_program_->draw(*screen_mesh_, 0, *cur_camera_comp);
    if (editor_->is_visible) editor_->viewport_fbo->unbind();
    
    glViewport(0, 0, args.width, args.height);
    
    //Editor
    if (editor_->is_visible)
    {
        editor_->set_main_dockspace();

        //Viewport
        const ImVec4 old_bg = ImGui::GetStyle().Colors[2];
        if (scene_->current_camera == scene_->player_camera) ImGui::GetStyle().Colors[2] = {0.5f, 0.2f, 0.2f, 0.7f};
        else scene_->current_camera = scene_->editor_camera.get();
        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoInputs);
        
        const ImVec2 pos_vp  = ImGui::GetCursorScreenPos();
        const ImVec2 size_vp = ImGui::GetContentRegionAvail();
        resize((int)size_vp.x, (int)size_vp.y);
        editor_->viewport_fbo->set_gui();
        
        ImGuizmo::SetRect(pos_vp.x, pos_vp.y, size_vp.x, size_vp.y);
        if (scene_->selected_entity != nullptr) scene_->selected_entity->transform->set_guizmo(scene_->current_camera, editor_->gizmo_operation);
        ImVec2 pos = ImGui::GetWindowPos();
        pos.x += ImGui::GetWindowContentRegionMin().x + 10;
        pos.y += ImGui::GetWindowContentRegionMin().y + 10;
        ImGui::End();
        if (scene_->current_camera == scene_->player_camera) ImGui::GetStyle().Colors[2] = old_bg;

        
        mouse_->set_gui();
            
        //Scene
        if (ImGui::Begin("Scene Graph ##ENTITIES_GRAPH"))
            scene_->set_scene_graph();
        ImGui::End();
        if (scene_->opened_gui)
        {
            ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
            if (ImGui::Begin("Scene Settings", &scene_->opened_gui))
                scene_->set_gui();
            ImGui::End();
        }
        //Inspector
        if (ImGui::Begin("Inspector ##ENTITIES_EDITOR"))
            scene_->set_entity_inspector();
        ImGui::End();

        //GBuffer
        if (editor_->show_g_buffer_inspector)
        {
            if (ImGui::Begin("GBuffer Inspector", &editor_->show_g_buffer_inspector))
                g_buffer_->set_gui();
            ImGui::End();
        }

        //Statistics
        if (editor_->show_statistics)
        {
            ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
            ImGui::SetNextWindowBgAlpha(0.35f);
            if (ImGui::Begin("Statistics", &editor_->show_statistics, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
            {
                ImGui::Text("%i fps (%.4f ms, Max: %i)", display_frame_count_, delta_time_ * 1000.0f, max_fps_plot_);
                if (ImGui::TreeNode("FPSHistogram", "FPS Histogram (past %i s)", std::size(fps_plot_)))
                {
                    ImGui::PlotHistogram("##FPSHistory", fps_plot_, std::size(fps_plot_), 0, nullptr, 0, (float)max_fps_plot_, {0, 80});
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Stats"))
                {
                    ImGui::Text("Uptime: %.3f s", glfwGetTime());
                    ImGui::Text("Window W: %i, H: %i", args.width, args.height);
                    ImGui::Text("Viewport W: %i, H: %i", cur_width, cur_height);
                    ImGui::Text("GBuffer W: %i, H: %i", g_buffer_->albedo_rough_texture->get_width(), g_buffer_->albedo_rough_texture->get_height());
                    ImGui::Text("Post Process W: %i, H: %i", post_process_program_->framebuffer_->attached_textures.front()->get_width(), post_process_program_->framebuffer_->attached_textures.front()->get_height());
                    ImGui::Text("Editor W: %i, H: %i", editor_->viewport_fbo->attached_textures.front()->get_width(), editor_->viewport_fbo->attached_textures.front()->get_height());
                    ImGui::TreePop();
                }
            }
            ImGui::End();
        }
        
        if (editor_->show_imgui_demo) ImGui::ShowDemoWindow();
    }
    editor_->render();

    args.scroll_x = 0.0f;
    args.scroll_y = 0.0f;
    
    glfwSwapBuffers(args.window);
    glfwPollEvents();
}

void Engine::resize(const int width, const int height)
{
    g_buffer_->resize(width, height);
    post_process_program_->resize(width, height);
    editor_->viewport_fbo->resize(width, height);
}


void Engine::set_models_to_mesh(Mesh* mesh)
{
    float vertices_cube[] = {
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // A 0
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // B 1
        1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // C 2
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // D 3
        
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // E 4
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // F 5
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // G 6
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // H 7
        
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // D 8
        -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // A 9
        -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // E 10
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // H 11
        
        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // B 12
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // C 13
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // G 14
        1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // F 15
        
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // A 16
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // B 17
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, // F 18
        -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // E 19
        
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // C 20
        -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // D 21
        -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // H 22
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f // G 23
    };
    unsigned int indices_cube[] = {
        // front and back
        0, 3, 2,
        2, 1, 0,
        4, 5, 6,
        6, 7, 4,
        // left and right
        11, 8, 9,
        9, 10, 11,
        12, 13, 14,
        14, 15, 12,
        // bottom and top
        16, 17, 18,
        18, 19, 16,
        20, 21, 22,
        22, 23, 20
    };
    mesh->add_model("Cube", vertices_cube, std::size(vertices_cube), indices_cube, std::size(indices_cube));

    float vertices_plane[] = {
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };
    unsigned int indices_plane[] = {
        0, 1, 2,
        2, 3, 0
    };
    mesh->add_model("Plane", vertices_plane, std::size(vertices_plane), indices_plane, std::size(indices_plane));
    
    float vertices_paper[] = {
        0.0f, 0.0f, 1.0f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, // 0
        -1.0f, -0.1f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 1
        1.0f, -0.1f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 2
        0.0f, 0.2f, -1.0f, 0.5f, 0.2f, 0.0f, 1.0f, 0.0f, // 3
        -0.2f, 0.0f, -1.0f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, // 4
        0.2f, 0.0f, -1.0f, 0.7f, 0.0f, 0.0f, 1.0f, 0.0f // 5
    };
    unsigned int indices_paper[] = {
        0, 4, 5,
        0, 3, 4,
        0, 5, 3,
        1, 4, 3,
        1, 3, 0,
        2, 3, 5,
        2, 0, 3
    };
    mesh->add_model("Paper plane", vertices_paper, std::size(vertices_paper), indices_paper, std::size(indices_paper));
}