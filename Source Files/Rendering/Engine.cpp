#include "Engine.h"

#include <algorithm>
#include <iostream>

#include "Entities/Components/CameraComponent.h"
#include "Entities/Components/LightComponent.h"
#include "Entities/Components/MaterialComponent.h"
#include "Entities/Components/TextureComponent.h"

Engine::Engine(const EngineArgs& args) : fps_plot_{}
{
    editor_ = new EditorManager(true);
    editor_->init_imgui(args.window);

    viewport_ = new GLint[2]{args.width, args.height};
    mouse_ = new Mouse(viewport_[0], viewport_[1], 5.0f);

    lights_ = {};

    set_icon(args.window, "Icon/green_tick.png");

    textures_ = TextureManager::get_instance();
    textures_->add_texture(new Texture("white1x1.png", GL_RGBA));
    textures_->add_texture(new Texture("black1x1.png", GL_RGBA));
    textures_->add_texture(new Texture("missing_texture.png", GL_RGBA));
    textures_->add_texture(new Texture("chill_guy.jpg", GL_RGB));
    textures_->add_texture(new Texture("container_diffuse.png", GL_RGBA));
    textures_->add_texture(new Texture("container_specular.png", GL_RGBA));
    textures_->add_texture(new Texture("shrek.png", GL_RGBA));
    textures_->add_texture(new Texture("test_fail.png", GL_RGBA));
    textures_->add_texture(new Texture("black_hole.jpg", GL_RGB));

    float vertices_cube[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // A 0
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // B 1
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // C 2
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // D 3

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // E 4
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // F 5
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // G 6
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // H 7

        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // D 8
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // A 9
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // E 10
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // H 11

        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // B 12
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // C 13
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // G 14
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // F 15

        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // A 16
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // B 17
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, // F 18
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // E 19

        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // C 20
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // D 21
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // H 22
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f // G 23
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

    float vertices_plane[] = {
        0.0f, 0.0f, 1.0f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, // 0
        -1.0f, -0.1f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 1
        1.0f, -0.1f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 2
        0.0f, 0.2f, -1.0f, 0.5f, 0.2f, 0.0f, 1.0f, 0.0f, // 3
        -0.2f, 0.0f, -1.0f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, // 4
        0.2f, 0.0f, -1.0f, 0.7f, 0.0f, 0.0f, 1.0f, 0.0f // 5
    };
    unsigned int indices_plane[] = {
        0, 4, 5,
        0, 3, 4,
        0, 5, 3,
        1, 4, 3,
        1, 3, 0,
        2, 3, 5,
        2, 0, 3
    };

    float vertices_floor[] = {
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };
    unsigned int indices_floor[] = {
        0, 1, 2,
        2, 3, 0
    };

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

    //Mesh
    player_camera_ = std::make_shared<Entity>("Player camera",
        new TransformComponent(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.3f)),
        new MeshComponent(vertices_cube, indices_cube,std::size(vertices_cube),std::size(indices_cube))
    );
    player_camera_->add_component(CAMERA, new CameraComponent(player_camera_->transform, {0.07f, 0.07f, 0.07f, 1.0f}));

    std::shared_ptr<Entity> obj1 = std::make_shared<Entity>("Cube",
        new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
        new MeshComponent(vertices_cube, indices_cube, std::size(vertices_cube), std::size(indices_cube))
    );
    obj1->add_component(MATERIAL, new MaterialComponent(glm::vec4(1.0, 1.0, 1.0, 1.0)));
    obj1->add_component(TEXTURE, new TextureComponent(textures_->get_texture(4)->get_handle(),textures_->get_texture(5)->get_handle()));

    std::shared_ptr<Entity> obj2 = std::make_shared<Entity>("Cube #2",
        new TransformComponent(glm::vec3(3.0f, 1.0f, 3.0f), glm::vec3(0.0f), glm::vec3(3.0f)),
        new MeshComponent(vertices_cube, indices_cube, std::size(vertices_cube), std::size(indices_cube))
    );
    obj2->add_component(TEXTURE, new TextureComponent(textures_->get_texture(3)->get_handle(), 1));
    obj1->add_child(obj2);

    std::shared_ptr<Entity> obj3 = std::make_shared<Entity>("Paper plane #2",
        new TransformComponent(glm::vec3(-2.0f, 1.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.5f))
        //new MeshComponent(vertices_plane, indices_plane, std::size(vertices_plane), std::size(indices_plane))
    );
    obj3->add_component(MATERIAL, new MaterialComponent(glm::vec4(0.0, 0.0, 1.0, 1.0)));
    obj2->add_child(obj3);

    std::shared_ptr<Entity> floor = std::make_shared<Entity>("Floor",
        new TransformComponent(glm::vec3(0.0f, -0.501f, 0.0f), glm::vec3(0.0f), glm::vec3(50.0f)),
        new MeshComponent(vertices_floor, indices_floor, std::size(vertices_floor), std::size(indices_floor))
    );
    floor->add_component(MATERIAL, new MaterialComponent(glm::vec4(0.15, 0.15, 0.15, 1.0)));

    std::shared_ptr<Entity> dir_light = std::make_shared<Entity>("Sun",
        new TransformComponent(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(-6.0f, -10, 6.0f), glm::vec3(0.5f)),
        new MeshComponent(vertices_cube, indices_cube, std::size(vertices_cube), std::size(indices_cube))
    );
    dir_light->add_component(LIGHT, new LightComponent(DIRECTIONAL, glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(0.8f)));
    lights_.push_back(dir_light);

    std::shared_ptr<Entity> point_light = std::make_shared<Entity>("Bulb",
        new TransformComponent(glm::vec3(3.0f, 5.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.35f)),
        new MeshComponent(vertices_cube, indices_cube, std::size(vertices_cube), std::size(indices_cube))
    );
    point_light->add_component(LIGHT, new LightComponent(POINT, glm::vec3(0.2f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.8f)));
    lights_.push_back(point_light);
    obj1->add_child(point_light);

    const VertexAttribute position("Position", 3);
    const VertexAttribute tex_coord("TexCoord", 2);
    const VertexAttribute normal("Normal", 3);
    VertexAttribute attributes[] = {position, tex_coord, normal};

    mesh_ = new Mesh(attributes, std::size(attributes));
    mesh_->add_entity(player_camera_);
    mesh_->add_entity(obj1);
    mesh_->add_entity(obj2);
    mesh_->add_entity(obj3);
    mesh_->add_entity(floor);
    for (const std::shared_ptr<Entity>& light : lights_) mesh_->add_entity(light);
    mesh_->compile();


    const Shader vert("shader.vert", GL_VERTEX_SHADER);
    const Shader frag("shader.frag", GL_FRAGMENT_SHADER);
    Shader shaders[]{vert, frag};
    program_ = new Program(shaders);


    //Screen Mesh
    VertexAttribute screen_attributes[] = {VertexAttribute("PosTex", 4)};
    screen_mesh_ = new Mesh(screen_attributes, std::size(screen_attributes));
    Entity screen("Screen",
                  new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
                  new MeshComponent(vertices_quad, indices_quad, std::size(vertices_quad), std::size(indices_quad)));
    screen_mesh_->add_entity(screen);
    screen_mesh_->compile();

    const Shader vert_screen("Screen/screen.vert", GL_VERTEX_SHADER);
    const Shader frag_screen("Screen/screen.frag", GL_FRAGMENT_SHADER);
    Shader shaders_screen[]{vert_screen, frag_screen};
    screen_program = new Program(shaders_screen);


    screen_texture_ = std::make_shared<Texture>(args.width, args.height, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
    screen_rbo_ = std::make_shared<Renderbuffer>(args.width, args.height, GL_DEPTH24_STENCIL8);

    screen_fbo_ = std::make_unique<Framebuffer>();
    screen_fbo_->attach_texture_2d(screen_texture_, GL_COLOR_ATTACHMENT0);
    screen_fbo_->attach_renderbuffer(screen_rbo_, GL_DEPTH_STENCIL_ATTACHMENT);
    screen_fbo_->check_completeness();
    textures_->add_texture(screen_fbo_->attached_textures.at(0).get());


    current_camera_ = editor_->is_visible ? editor_->camera : player_camera_.get();
    
    is_fullscreen_ = false;
    fullscreen_toggle_ = true;
    camera_toggle_ = true;
    can_escape_ = true;
    entity_select_ = 0;

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
    {
        screen_fbo_->resize(args.width, args.height);
        screen_rbo_->resize(args.width, args.height);
        screen_fbo_->attach_renderbuffer(screen_rbo_, GL_DEPTH_STENCIL_ATTACHMENT);
    }
    viewport_[0] = args.width;
    viewport_[1] = args.height;

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
        {
            screen_fbo_->resize(args.width, args.height);
            screen_rbo_->resize(args.width, args.height);
            screen_fbo_->attach_renderbuffer(screen_rbo_, GL_DEPTH_STENCIL_ATTACHMENT);
        }
    }
    else if (!editor_->toggle_visibility && glfwGetKey(args.window, GLFW_KEY_HOME) == GLFW_RELEASE) editor_->toggle_visibility = true;

    //INS
    if (glfwGetKey(args.window, GLFW_KEY_INSERT) == GLFW_PRESS && camera_toggle_)
    {
        camera_toggle_ = false;
        if (current_camera_ != editor_->camera) current_camera_ = editor_->camera;
        else if (current_camera_ != player_camera_.get()) current_camera_ = player_camera_.get();
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


    //Camera update
    dynamic_cast<CameraComponent*>(current_camera_->components->at(CAMERA))->move(args.window, delta_time_);
    if (!mouse_->is_visible) dynamic_cast<CameraComponent*>(current_camera_->components->at(CAMERA))->mouse_move(*mouse_, delta_time_);
    
    //if (args.io->WantCaptureMouse) return;
}

void Engine::render(const EngineArgs& args)
{
    editor_->new_frame();

    CameraComponent* cur_camera_comp = dynamic_cast<CameraComponent*>(current_camera_->components->at(CAMERA));
    const int cur_width = screen_fbo_->attached_textures.at(0)->get_width();
    const int cur_height = screen_fbo_->attached_textures.at(0)->get_height();
    
    screen_fbo_->bind();
    glViewport(0, 0, cur_width, cur_height);
    glClearColor(cur_camera_comp->clear_color[0], cur_camera_comp->clear_color[1], cur_camera_comp->clear_color[2], cur_camera_comp->clear_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Draw
    if (cur_camera_comp->is_wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    program_->draw(mesh_, lights_, current_camera_, cur_width, cur_height);
    
    if (cur_camera_comp->is_wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    screen_fbo_->unbind();
    glViewport(0, 0, args.width, args.height);
    
    //Screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!editor_->is_visible)
    {
        glDisable(GL_DEPTH_TEST);
        screen_program->draw_screen(screen_mesh_, screen_fbo_->attached_textures.at(0)->get_handle()); //static_cast<unsigned int>(cur_camera_comp->clear_color[0] * 255)
        glEnable(GL_DEPTH_TEST);
    }
    
    //Editor
    if (editor_->is_visible)
    {
        editor_->set_main_dockspace();

        const ImVec4 old_bg = ImGui::GetStyle().Colors[2];
        if (current_camera_ == player_camera_.get()) ImGui::GetStyle().Colors[2] = {0.5f, 0.2f, 0.2f, 0.7f};
        ImGui::Begin("Viewport");
        screen_fbo_->set_gui();
        ImVec2 pos = ImGui::GetWindowPos();
        pos.x += ImGui::GetWindowContentRegionMin().x + 10;
        pos.y += ImGui::GetWindowContentRegionMin().y + 10;
        ImGui::End();
        if (current_camera_ == player_camera_.get()) ImGui::GetStyle().Colors[2] = old_bg;

        if (editor_->show_statistics)
        {
            ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
            ImGui::SetNextWindowBgAlpha(0.35f);
            if (ImGui::Begin("Statistics", &editor_->show_statistics, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
            {
                ImGui::Text("%i fps (%.4f ms, Max: %i)", display_frame_count_, delta_time_ * 1000.0f, max_fps_plot_);
                if (ImGui::TreeNode("FPS Histogram"))
                {
                    ImGui::PlotHistogram("##FPSHistory", fps_plot_, std::size(fps_plot_), 0, nullptr, 0, (float)max_fps_plot_, {0, 80});
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Stats"))
                {
                    ImGui::Text("Uptime: %.3f s", glfwGetTime());
                    ImGui::Text("Window W: %i, H: %i", args.width, args.height);
                    ImGui::Text("Viewport W: %i, H: %i", cur_width, cur_height);
                    ImGui::TreePop();
                }
                ImGui::End();
            }
        }

        /*if (current_camera_ == player_camera_.get())
        {
            //ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
            ImGui::SetNextWindowBgAlpha(0.35f);
            if (ImGui::Begin("InGame", &editor_->show_statistics, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
            {
                ImGui::PushFont(nullptr, ImGui::GetStyle().FontSizeBase * 2.0f);
                ImGui::Text("  IN GAME  ");
                ImGui::PopFont();
                ImGui::End();
            }
        }*/

        ImGui::Begin("Editor Camera");
        dynamic_cast<CameraComponent*>(editor_->camera->components->at(CAMERA))->set_gui();
        ImGui::End();
        
        mouse_->set_gui();

        if (ImGui::Begin("Entity Inspector ##Inspector"))
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
            ImGui::BeginChild("##Entities", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY);
            int count = 0;
            editor_->set_graph_children(mesh_->entities, &entity_select_, &count);
            ImGui::EndChild();
            ImGui::PopStyleColor();
        
            ImGui::BeginChild("##EntityInspect", {0, 0}, ImGuiChildFlags_Borders);
            mesh_->entities.at(entity_select_)->set_gui();
            ImGui::EndChild();
        }
        ImGui::End();
        
        if (editor_->show_imgui_demo) ImGui::ShowDemoWindow();
    }
    editor_->render();
    
    glfwSwapBuffers(args.window);
    glfwPollEvents();
}