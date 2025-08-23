#include "EditorManager.h"

#include "../Entities/Components/CameraComponent.h"

EditorManager::EditorManager(const bool visible)
{
    is_visible = visible;
    toggle_visibility = true;

    camera = new Entity("Editor camera",
        new TransformComponent(glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f))
        );
    camera->add_component(CAMERA, new CameraComponent(camera->transform));
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
}

void EditorManager::new_frame()
{
    if (!is_visible) return;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void EditorManager::render()
{
    if (!is_visible) return;
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
