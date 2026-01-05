#include "ImGuiLayer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glfw3.h>
#include <engine/utils/FileDialogs.h>


namespace editor
{
    ImGuiLayer::ImGuiLayer()
    {
    }

    void ImGuiLayer::shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }


    void ImGuiLayer::init(GLFWwindow* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        imgui_io = &ImGui::GetIO(); (void)imgui_io;
        imgui_io->FontGlobalScale = 2.0f;
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = {0.08f, 0.1f, 0.13f, 0.9f};

        imgui_io->Fonts->AddFontFromFileTTF("editor/assets/fonts/Inter_24pt-SemiBold.ttf", 14);
        set_red_style();

        //ImGuizmo::SetOrthographic(false);


        imgui_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        imgui_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    }


    void ImGuiLayer::on_begin_frame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //ImGuizmo::BeginFrame();
    }

    void ImGuiLayer::on_end_frame()
    {
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


    void ImGuiLayer::set_red_style()
    {
        ImVec4* colors = ImGui::GetStyle().Colors;
        /*colors[ImGuiCol_TextDisabled]           = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
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
        colors[ImGuiCol_DockingPreview]         = ImVec4(0.98f, 0.26f, 0.26f, 0.70f);*/

        colors[ImGuiCol_WindowBg]               = ImVec4(0.07f, 0.08f, 0.11f, 0.7f);


        ImGui::GetStyle().WindowRounding = 10;
        ImGui::GetStyle().ChildRounding = 10;
        ImGui::GetStyle().FrameRounding = 8;
        ImGui::GetStyle().PopupRounding = 12;
        ImGui::GetStyle().GrabRounding = 4;

    }
}
