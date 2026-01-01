#include "EditorApplication.h"

#include "engine/rendering/Renderer.h"
#include "engine/world/Entity.h"
#include "gui/panels/PerformancePanel.h"
#include "gui/panels/SceneGraphPanel.h"
#include "gui/panels/ViewportPanel.h"

#include <memory>
#include <engine/utils/FileDialogs.h>
#include <engine/world/components/MeshComponent.h>
#include <engine/world/components/MaterialComponent.h>
#include <engine/world/components/LightComponent.h>

#include "imgui.h"


namespace editor
{
    EditorApplication::EditorApplication()
    {
        viewport_panel_ = std::make_unique<ViewportPanel>();
        performance_panel_ = std::make_unique<PerformancePanel>();
        scene_graph_panel_ = std::make_unique<SceneGraphPanel>();
        entity_inspector_panel_ = std::make_unique<EntityInspectorPanel>();
        project_modal_ = std::make_unique<ProjectsModalPanel>();
        project_new_modal_ = std::make_unique<ProjectNewModal>();

        show_imgui_demo_ = false;
        show_projects_modal_ = true;
        show_projects_new_modal_ = false;
    }

    void EditorApplication::on_start()
    {
        Scene* sample_scene = create_sample_scene();
    }

    void EditorApplication::on_update(const float delta_time)
    {

    }

    void EditorApplication::on_render(engine::Renderer& renderer)
    {
        draw_dockspace();


        project_modal_->draw(*this, &show_projects_modal_, &show_projects_new_modal_);
        project_new_modal_->draw(*this, &show_projects_new_modal_);

        viewport_panel_->draw(renderer);
        performance_panel_->draw(viewport_panel_->pos, renderer);

        scene_graph_panel_->draw(*scene_manager->get_current_scene());
        entity_inspector_panel_->draw(scene_graph_panel_->selected_entity, *resource_manager->get_texture_manager());


        if (show_imgui_demo_) ImGui::ShowDemoWindow(&show_imgui_demo_);
    }
    
    void EditorApplication::reset()
    {
        resource_manager.get()->reset();
        scene_manager.get()->reset();
        scene_graph_panel_->selected_entity = nullptr;
    }


    Scene* EditorApplication::create_sample_scene()
    {
        Scene* scene = scene_manager->create_scene("Untitled", true);

        Entity* e = scene->create_entity("Cube");
        e->add_component<MeshComponent>(0, GL_TRIANGLES, resource_manager->get_mesh_manager());
        e->add_component<MaterialComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.75f);

        std::unique_ptr<Entity> light = std::make_unique<Entity>("Light",
            new TransformComponent(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f))
        );
        light->transform->scale *= 0.2f;
        light->transform->position = glm::vec3(3.0f, 2.0f, 2.5f);
        light->add_component<MeshComponent>(0, 4, resource_manager->get_mesh_manager());
        light->add_component<LightComponent>(lights::LIGHT_TYPE::POINT, glm::vec3(1.0f, 1.0f, 1.0f));
        light->get_component<LightComponent>()->intensity = 5.0f;
        scene->add_entity(std::move(light));

        return scene;
    }

    void EditorApplication::draw_dockspace()
    {
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Project"))
            {
                if (ImGui::MenuItem("New Project", "CTRL+N"))
                {
                    show_projects_new_modal_ = true;
                }

                if (ImGui::MenuItem("Open...", "CTRL+O"))
                {
                    const std::string path = engine::FileDialogs::save_file("Rainfall Project (*.rainp)\0*.rainp\0");
                    if (!path.empty())
                        current_project->load(*this, path);
                }
                if (ImGui::MenuItem("Save", "CTRL+S") && current_project->save_path != "N/A")
                {
                    current_project->save(*this, current_project->save_path);
                }
                if (ImGui::MenuItem("Save As...", "CTRL+SHIFT+S"))
                {
                    const std::string path = engine::FileDialogs::save_file("Rainfall Project (*.rainp)\0*.rainp\0");
                    if (!path.empty())
                        current_project->save(*this, path);
                }

                if (ImGui::MenuItem("Open Modal"))
                {
                    show_projects_modal_ = true;
                }

                ImGui::Separator();

                if (ImGui::BeginMenu("Loaded scenes"))
                {
                    for (const auto & [key, value] : *scene_manager->get_all_scenes())
                    {
                        if (ImGui::Selectable(key.c_str()))
                            scene_manager->switch_to(key);
                    }
                    ImGui::EndMenu();
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Exit"))
                    stop();

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Scene"))
            {
                if (ImGui::MenuItem("New", "CTRL+N"))
                    //project.current_scene->reset();

                if (ImGui::MenuItem("Open..", "CTRL+O"))
                {
                    //project.load_scene_dialog();
                }

                //if (ImGui::MenuItem("Save", "CTRL+S") && project.current_scene->save_path != "N/A")
                //{
                    //project.current_scene->save(project.current_scene->save_path);
                //}
                if (ImGui::MenuItem("Save As..", "CTRL+SHIFT+S"))
                {
                    const std::string path = engine::FileDialogs::save_file("Rainfall Scene (*.rain)\0*.rain\0");
                    //if (!path.empty())
                        //project.current_scene->save(path);
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Options"))
            {
                ImGui::SeparatorText("Editor Options");
                //ImGui::SliderFloat("UI Scale", &imgui_io->FontGlobalScale, 1.0f, 3.0f, "%.1f");
                ImGui::ColorEdit4("Window Bg", (float*)&ImGui::GetStyle().Colors[ImGuiCol_WindowBg], ImGuiColorEditFlags_AlphaBar);

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window"))
            {
                ImGui::SeparatorText("Available Windows");
                //ImGui::MenuItem("Show Statistics", "CTRL+P", &show_statistics);
                //ImGui::MenuItem("Show GBuffer Inspector", "CTRL+G", &show_g_buffer_inspector);

                //ImGui::Separator();
                ImGui::MenuItem("Show ImGui Demo", "CTRL+D", &show_imgui_demo_);

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
}
