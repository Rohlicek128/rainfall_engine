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

    selected_entity = nullptr;
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

void EditorManager::set_scene_graph(Scene* scene)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
    if (ImGui::BeginChild("##Entities", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeY))
    {
        ImGui::SeparatorText(scene->name.c_str());
        if (ImGui::BeginChild("##EntitiesInner", {-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing()}, ImGuiChildFlags_ResizeY))
        {
            set_graph_children(scene->root_entity->children, selected_entity);
        }
        ImGui::EndChild();

        if (ImGui::Button("Add Empty", {-FLT_MIN, ImGui::GetTextLineHeightWithSpacing()}))
        {
            std::unique_ptr<Entity> empty = std::make_unique<Entity>("Empty", new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
            scene->add_entity(std::move(empty));
        }
        
        ImGui::Button("Root", {-FLT_MIN, ImGui::GetTextLineHeightWithSpacing()});
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CHILD_DND"))
            {
                IM_ASSERT(payload->DataSize == sizeof(Entity*));
                Entity* payload_entity = *(Entity**)payload->Data;
                if (payload_entity != nullptr && payload_entity->parent != nullptr)
                {
                    payload_entity->parent->remove_child(payload_entity);
                    scene->root_entity->add_child(payload_entity);
                }
            }
            ImGui::EndDragDropTarget();
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void EditorManager::set_entity_inspector()
{
    ImGui::BeginChild("##EntityInspect", {0, 0}, ImGuiChildFlags_Borders);
    if (selected_entity != nullptr) selected_entity->set_gui();
    ImGui::EndChild();
}

void EditorManager::set_graph_children(const std::vector<Entity*>& children, Entity*& selected)
{
    for (int i = 0; i < children.size(); ++i)
    {
        if (children.at(i)->parent == nullptr) continue;
        
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (selected == children.at(i)) node_flags |= ImGuiTreeNodeFlags_Selected;
        if (children.at(i)->children.empty()) node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet;
        
        const bool opened = ImGui::TreeNodeEx((children.at(i)->name + "##ENTITY" + std::to_string(i)).c_str(), node_flags);
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            selected = children.at(i);

        if (ImGui::BeginPopupContextItem())
        {
            ImGui::SeparatorText("Options");
            ImGui::Button("Delete");
            ImGui::EndPopup();
        }
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            ImGui::SetDragDropPayload("CHILD_DND", &selected, sizeof(Entity*));
            ImGui::Text(selected->name.c_str());
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CHILD_DND"))
            {
                IM_ASSERT(payload->DataSize == sizeof(Entity*));
                Entity* payload_entity = *(Entity**)payload->Data;
                if (payload_entity != nullptr) children.at(i)->add_child(payload_entity);
            }
            ImGui::EndDragDropTarget();
        }
        if (opened)
        {
            set_graph_children(children.at(i)->children, selected);
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
