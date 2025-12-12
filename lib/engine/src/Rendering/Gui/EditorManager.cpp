#include "EditorManager.h"

#include "ImGuizmo.h"
#include "../../Organization/Project.h"
#include "../Entities/Components/CameraComponent.h"
#include "engine/world/Scene.h"

#include "../../Utils/FileDialogs.h"

EditorManager::EditorManager(const bool visible, int width, int height)
{
    imgui_io = nullptr;
    is_visible = visible;
    toggle_visibility = true;
    show_imgui_demo = false;
    show_statistics = true;
    show_g_buffer_inspector = false;

    gizmo_operation = ImGuizmo::OPERATION::TRANSLATE;

    viewport_fbo = std::make_unique<Framebuffer>();
    viewport_fbo->attach_texture_2d(std::make_unique<Texture>(width, height, GL_RGBA8, GL_RGBA), GL_COLOR_ATTACHMENT0);
    viewport_fbo->set_draw_buffers();
    viewport_fbo->check_completeness();
}

void EditorManager::init_imgui(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    imgui_io = &ImGui::GetIO(); (void)imgui_io;
    imgui_io->FontGlobalScale = 2.0f;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = {0.08f, 0.1f, 0.13f, 0.9f};

    imgui_io->Fonts->AddFontFromFileTTF("assets/Fonts/CascadiaMono-SemiBold.ttf", 13);
    //set_red_style();

    ImGuizmo::SetOrthographic(false);
}

void EditorManager::set_main_dockspace(const engine::EngineArgs& args, Project& project)
{
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Project"))
        {
            if (ImGui::MenuItem("New Project", "CTRL+N"))
            {
                project.reset();
                project.add_empty_scene();
            }

            if (ImGui::MenuItem("Open..", "CTRL+O"))
            {
                const std::string path = FileDialogs::save_file("Rainfall Project (*.rainp)\0*.rainp\0");
                if (!path.empty())
                    project.load(path);
            }
            if (ImGui::MenuItem("Save", "CTRL+S") && project.current_scene->save_path != "N/A")
            {
                project.save(project.path);
            }
            if (ImGui::MenuItem("Save As..", "CTRL+SHIFT+S"))
            {
                const std::string path = FileDialogs::save_file("Rainfall Project (*.rainp)\0*.rainp\0");
                if (!path.empty())
                    project.save(path);
            }

            ImGui::Separator();

            if (ImGui::BeginMenu("Loaded scenes"))
            {
                for (unsigned int i = 0; i < project.scenes.size(); ++i)
                {
                    if (ImGui::Selectable(project.scenes.at(i)->name.c_str()))
                        project.current_scene = project.scenes.at(i).get();
                }

                ImGui::EndMenu();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Exit"))
                glfwSetWindowShouldClose(args.window, true);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Scene"))
        {
            if (ImGui::MenuItem("New", "CTRL+N"))
                project.current_scene->reset();

            if (ImGui::MenuItem("Open..", "CTRL+O"))
            {
                project.load_scene_dialog();
            }

            if (ImGui::MenuItem("Save", "CTRL+S") && project.current_scene->save_path != "N/A")
            {
                project.current_scene->save(project.current_scene->save_path);
            }
            if (ImGui::MenuItem("Save As..", "CTRL+SHIFT+S"))
            {
                const std::string path = FileDialogs::save_file("Rainfall Scene (*.rain)\0*.rain\0");
                if (!path.empty())
                    project.current_scene->save(path);
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options"))
        {
            ImGui::SeparatorText("Editor Options");
            ImGui::SliderFloat("UI Scale", &imgui_io->FontGlobalScale, 1.0f, 3.0f, "%.1f");
            ImGui::ColorEdit4("Window Bg", (float*)&ImGui::GetStyle().Colors[ImGuiCol_WindowBg], ImGuiColorEditFlags_AlphaBar);

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window"))
        {
            ImGui::SeparatorText("Available Windows");
            ImGui::MenuItem("Show Statistics", "CTRL+P", &show_statistics);
            ImGui::MenuItem("Show GBuffer Inspector", "CTRL+G", &show_g_buffer_inspector);

            ImGui::Separator();
            ImGui::MenuItem("Show ImGui Demo", "CTRL+D", &show_imgui_demo);

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void EditorManager::set_red_style()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.07f, 0.08f, 0.11f, 0.92f);
    colors[ImGuiCol_Border]                 = ImVec4(0.36f, 0.36f, 0.36f, 0.35f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.27f, 0.16f, 0.16f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.69f, 0.20f, 0.20f, 0.40f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.86f, 0.23f, 0.23f, 0.67f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.85f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.88f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.98f, 0.26f, 0.26f, 0.49f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.78f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.97f, 0.36f, 0.36f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(1.00f, 0.29f, 0.29f, 0.55f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(1.00f, 0.23f, 0.23f, 0.80f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.53f, 0.30f, 0.30f, 0.50f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.75f, 0.10f, 0.10f, 0.78f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.75f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.98f, 0.26f, 0.26f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.98f, 0.26f, 0.26f, 0.95f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.58f, 0.18f, 0.18f, 0.86f);
    colors[ImGuiCol_TabSelected]            = ImVec4(0.68f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_TabSelectedOverline]    = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_TabDimmed]              = ImVec4(0.15f, 0.07f, 0.07f, 0.97f);
    colors[ImGuiCol_TabDimmedSelected]      = ImVec4(0.42f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.98f, 0.26f, 0.26f, 0.70f);
}

void EditorManager::new_frame()
{
    if (!is_visible) return;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void EditorManager::render()
{
    if (!is_visible) return;
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (imgui_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
