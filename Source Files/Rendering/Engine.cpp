#include "Engine.h"

#include <algorithm>
#include <iostream>

#include "Entities/Components/CameraComponent.h"
#include "Entities/Components/LightComponent.h"
#include "Entities/Components/MaterialComponent.h"
#include "Entities/Components/TextureComponent.h"

Engine::Engine(const EngineArgs& args)
{
    editor_ = new EditorManager(true);
    editor_->init_imgui(args.window);
    
    viewport_ = new GLint[2] {args.width, args.height};
    mouse_ = new Mouse(viewport_[0], viewport_[1], 8.0f);

    lights_ = {};

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
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f, // A 0
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f, // B 1
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f, // C 2
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f, // D 3
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f, // E 4
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f, // F 5
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f, // G 6
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f, // H 7
 
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f, // D 8
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f, // A 9
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f, // E 10
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f, // H 11
        
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,  // B 12
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,  // C 13
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,  // G 14
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  // F 15
 
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f, // A 16
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,  // B 17
        0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,  // F 18
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f, // E 19
        
        0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,  1.0f,  0.0f, // C 20
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f, // D 21
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f, // H 22
        0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  1.0f,  0.0f  // G 23
    };
    unsigned int indices_cube[] = {
        // front and back
        0, 3, 2,
        2, 1, 0,
        4, 5, 6,
        6, 7 ,4,
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
        0.0f,  0.0f,  1.0f,   0.5f, 1.0f,   0.0f, 1.0f, 0.0f,  // 0
       -1.0f, -0.1f, -1.0f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,  // 1
        1.0f, -0.1f, -1.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,  // 2
        0.0f,  0.2f, -1.0f,   0.5f, 0.2f,   0.0f, 1.0f, 0.0f,  // 3
       -0.2f,  0.0f, -1.0f,   0.3f, 0.0f,   0.0f, 1.0f, 0.0f,  // 4
        0.2f,  0.0f, -1.0f,   0.7f, 0.0f,   0.0f, 1.0f, 0.0f   // 5
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
        1.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f,  1.0f,  0.0f,
        1.0f, 0.0f, -1.0f,  1.0f, 0.0f,   0.0f,  1.0f,  0.0f,
        -1.0f, 0.0f, -1.0f,   1.0f, 1.0f,   0.0f,  1.0f,  0.0f,
        -1.0f, 0.0f, 1.0f,  0.0f, 1.0f,   0.0f,  1.0f,  0.0f
    };
    unsigned int indices_floor[] = {
        0, 1, 2,
        2, 3, 0
    };

    player_camera_ = std::make_shared<Entity>(Entity("Player camera",
        new TransformComponent(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.3f)),
        new MeshComponent(vertices_cube, indices_cube, std::size(vertices_cube), std::size(indices_cube))
        ));
    player_camera_->add_component(CAMERA, new CameraComponent(player_camera_->transform));
    
    Entity obj1("Cube",
        new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
        new MeshComponent(vertices_cube, indices_cube, std::size(vertices_cube), std::size(indices_cube))
        );
    obj1.add_component(MATERIAL, new MaterialComponent(glm::vec4(1.0, 1.0, 1.0, 1.0)));
    obj1.add_component(TEXTURE, new TextureComponent(textures_->get_texture(4)->get_handle(), textures_->get_texture(5)->get_handle()));

    Entity obj2("Cube #2",
        new TransformComponent(glm::vec3(3.0f, 1.0f, 3.0f), glm::vec3(0.0f), glm::vec3(3.0f)),
        new MeshComponent(vertices_cube, indices_cube, std::size(vertices_cube), std::size(indices_cube))
        );
    obj2.add_component(MATERIAL, new MaterialComponent(glm::vec4(0.0, 1.0, 0.0, 1.0)));
    
    Entity obj3("Paper plane #2",
       new TransformComponent(glm::vec3(-2.0f, 1.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.5f))
       //new MeshComponent(vertices_plane, indices_plane, std::size(vertices_plane), std::size(indices_plane))
       );
    obj3.add_component(MATERIAL, new MaterialComponent(glm::vec4(0.0, 0.0, 1.0, 1.0)));

    Entity floor("Floor",
       new TransformComponent(glm::vec3(0.0f, -0.501f, 0.0f), glm::vec3(0.0f), glm::vec3(50.0f)),
       new MeshComponent(vertices_floor, indices_floor, std::size(vertices_floor), std::size(indices_floor))
       );
    floor.add_component(MATERIAL, new MaterialComponent(glm::vec4(0.15, 0.15, 0.15, 1.0)));
    
    Entity dir_light("Sun",
       new TransformComponent(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(-6.0f, -10, 6.0f), glm::vec3(0.5f)),
       new MeshComponent(vertices_cube, indices_cube, std::size(vertices_cube), std::size(indices_cube))
       );
    dir_light.add_component(LIGHT, new LightComponent(DIRECTIONAL, glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(0.8f)));
    lights_.push_back(std::make_shared<Entity>(dir_light));

    Entity point_light("Bulb",
       new TransformComponent(glm::vec3(3.0f, 5.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.35f)),
       new MeshComponent(vertices_cube, indices_cube, std::size(vertices_cube), std::size(indices_cube))
       );
    point_light.add_component(LIGHT, new LightComponent(POINT, glm::vec3(0.2f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.8f)));
    lights_.push_back(std::make_shared<Entity>(point_light));
    
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

    
    const Shader vert("Source Files/Rendering/Shaders/GLSL/shader.vert", GL_VERTEX_SHADER);
    const Shader frag("Source Files/Rendering/Shaders/GLSL/shader.frag", GL_FRAGMENT_SHADER);
    Shader shaders[]{vert, frag};
    program_ = new Program(shaders);
    

    current_camera_ = editor_->is_visible ? editor_->camera : player_camera_.get();
    
    is_wireframe_ = false;
    is_fullscreen_ = false;
    fullscreen_toggle_ = true;
    can_escape_ = true;
    entity_select_ = 0;

    delta_time_ = 0.0f;
    last_time_ = 0.0;

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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
}

void Engine::update(const EngineArgs& args)
{
    update_delta_time();
    mouse_->pos_x = args.mouse_x;
    mouse_->pos_y = args.mouse_y;
    viewport_[2] = args.width;
    viewport_[3] = args.height;

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
    }
    else if (!editor_->toggle_visibility && glfwGetKey(args.window, GLFW_KEY_HOME) == GLFW_RELEASE) editor_->toggle_visibility = true;

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
    if (editor_->is_visible && current_camera_ != editor_->camera) current_camera_ = editor_->camera;
    else if (!editor_->is_visible && current_camera_ != player_camera_.get()) current_camera_ = player_camera_.get();

    dynamic_cast<CameraComponent*>(current_camera_->components->at(CAMERA))->move(args.window, delta_time_);
    if (!mouse_->is_visible) dynamic_cast<CameraComponent*>(current_camera_->components->at(CAMERA))->mouse_move(*mouse_, delta_time_);
    
    //if (args.io->WantCaptureMouse) return;
}

void Engine::render(const EngineArgs& args)
{
    CameraComponent* cur_camera_comp = dynamic_cast<CameraComponent*>(current_camera_->components->at(CAMERA));
    glClearColor(cur_camera_comp->clear_color[0], cur_camera_comp->clear_color[1], cur_camera_comp->clear_color[2], cur_camera_comp->clear_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    editor_->new_frame();

    //Draw
    if (is_wireframe_) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    program_->draw(mesh_, lights_, current_camera_, viewport_);

    if (is_wireframe_) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Editor
    if (editor_->is_visible)
    {
        ImGui::Begin("Info");
        std::string str = std::to_string(1.0 / delta_time_) + " fps (" + std::to_string(delta_time_ * 1000.0) + " ms)";
        ImGui::Text(str.c_str());
        ImGui::Text("Uptime: %.3f s", glfwGetTime());
        str = "Width: " + std::to_string(args.width) + ", Height: " + std::to_string(args.height);
        ImGui::Text(str.c_str());
        ImGui::Checkbox("Wireframe", &is_wireframe_);
        ImGui::SliderFloat("UI Scale", &editor_->imgui_io->FontGlobalScale, 1.0f, 3.0f);
        ImGui::End();

        ImGui::Begin("Editor Camera");
        cur_camera_comp->set_gui();
        ImGui::End();
        
        mouse_->set_gui();

        ImGui::Begin("Entity Inspector", 0, ImGuiWindowFlags_NoDecoration);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
        if (ImGui::BeginChild("##Entities", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY))
        {
            for (int i = 0; i < mesh_->entities.size(); ++i)
            {
                if (ImGui::Selectable((mesh_->entities.at(i)->name + "##ENTITY" + std::to_string(i)).c_str(), entity_select_ == i)) entity_select_ = i;
                if (entity_select_ == i) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndChild();
        }
        ImGui::PopStyleColor();
        ImGui::Separator();
        mesh_->entities.at(entity_select_)->set_gui();
        ImGui::End();

        //ImGui::ShowDemoWindow();
    }
    editor_->render();
    
    glfwSwapBuffers(args.window);
    glfwPollEvents();
}