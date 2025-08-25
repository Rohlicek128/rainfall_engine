#include "EditorManager.h"

#include "../Entities/Components/CameraComponent.h"

EditorManager::EditorManager(const bool visible)
{
    imgui_io = nullptr;
    is_visible = visible;
    toggle_visibility = true;
    show_imgui_demo = false;
    show_statistics = true;

    camera = new Entity("Editor camera",
        new TransformComponent(glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f))
        );
    camera->add_component(CAMERA, new CameraComponent(camera->transform, {0.25f, 0.25f, 0.25f, 1.0f}));
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

void EditorManager::set_main_dockspace()
{
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            ImGui::SeparatorText("Editor Options");
            ImGui::SliderFloat("UI Scale", &imgui_io->FontGlobalScale, 1.0f, 3.0f, "%.1f");
            ImGui::ColorEdit4("Window Bg", (float*)&ImGui::GetStyle().Colors[2], ImGuiColorEditFlags_AlphaBar);
            
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

void EditorManager::set_graph_children(const std::vector<std::shared_ptr<Entity>>& children, std::weak_ptr<Entity>& selected, const bool is_root)
{
    for (int i = 0; i < children.size(); ++i)
    {
        if (is_root && children.at(i)->parent.lock()) continue;
        
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (selected.lock() == children.at(i)) node_flags |= ImGuiTreeNodeFlags_Selected;
        if (children.at(i)->children.empty()) node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
        
        const bool opened = ImGui::TreeNodeEx((children.at(i)->name + "##ENTITY" + std::to_string(i)).c_str(), node_flags);
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) selected = children.at(i)->weak_from_this();

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            ImGui::SetDragDropPayload("CHILD_DND", &selected, sizeof(std::weak_ptr<Entity>));
            ImGui::Text(selected.lock()->name.c_str());
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CHILD_DND"))
            {
                IM_ASSERT(payload->DataSize == sizeof(std::weak_ptr<Entity>));
                std::weak_ptr<Entity> payload_entity = *(std::weak_ptr<Entity>*)payload->Data;
                if (std::shared_ptr<Entity> old_payload = payload_entity.lock()) children.at(i)->add_child(old_payload);
            }
            ImGui::EndDragDropTarget();
        }
        if (opened)
        {
            set_graph_children(children.at(i)->children, selected, false);
            ImGui::TreePop();
        }
    }
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

    if (imgui_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}
