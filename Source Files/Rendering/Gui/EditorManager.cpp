#include "EditorManager.h"

#include "../../Imgui/ImGuizmo.h"
#include "../Entities/Components/CameraComponent.h"

EditorManager::EditorManager(const bool visible, int width, int height)
{
    imgui_io = nullptr;
    is_visible = visible;
    toggle_visibility = true;
    show_imgui_demo = false;
    show_statistics = true;

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

    ImGuizmo::SetOrthographic(false);
}

void EditorManager::set_main_dockspace()
{
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    if (ImGui::BeginMainMenuBar())
    {
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

            ImGui::Separator();
            ImGui::MenuItem("Show ImGui Demo", "CTRL+D", &show_imgui_demo);
            
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
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
