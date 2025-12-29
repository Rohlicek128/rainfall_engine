#include "EditorApplication.h"
#include "engine/rendering/Renderer.h"
#include "gui/panels/PerformancePanel.h"
#include "gui/panels/ViewportPanel.h"

#include <memory>
#include <engine/utils/FileDialogs.h>
#include <engine/world/components/MeshComponent.h>
#include <engine/world/components/MaterialComponent.h>
#include <engine/world/components/LightComponent.h>

#include "imgui.h"


namespace editor
{
    void EditorApplication::on_start()
    {
        working_dir_ = "C:\\Files\\Code\\C++\\rainfall_engine\\game\\";

        viewport_panel_ = std::make_unique<ViewportPanel>();
        performance_panel_ = std::make_unique<PerformancePanel>();


        scene_manager->create_scene("editor", true);
        //scene_manager->load_scene(working_dir_ + "saved\\Another.rain", true);

        auto e = scene_manager->get_current_scene()->create_entity("Cube");
        e->add_component<MeshComponent>(0, GL_TRIANGLES, resource_manager->get_mesh_manager());
        e->add_component<MaterialComponent>(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), 0.5f);

        auto light = scene_manager->get_current_scene()->create_entity("Light");
        light->transform->scale *= 0.2f;
        light->transform->position.x = 3.0f;
        light->transform->position.z = 2.0f;
        light->add_component<MeshComponent>(0, 4, resource_manager->get_mesh_manager());
        light->add_component<LightComponent>(lights::LIGHT_TYPE::POINT, glm::vec3(1.0f, 1.0f, 1.0f));
        light->get_component<LightComponent>()->intensity = 5.0f;
        scene_manager->get_current_scene()->add_light(light);
    }

    void EditorApplication::on_update(const float delta_time)
    {

    }

    void EditorApplication::on_render(engine::Renderer& renderer)
    {
        draw_dockspace();

        viewport_panel_->draw(*renderer.get_render_fbo());
        performance_panel_->draw(viewport_panel_->pos, renderer);
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
                    //project.reset();
                    //project.add_empty_scene();
                }

                if (ImGui::MenuItem("Open..", "CTRL+O"))
                {
                    const std::string path = engine::FileDialogs::save_file("Rainfall Project (*.rainp)\0*.rainp\0");
                    //if (!path.empty())
                        //project.load(path);
                }
                //if (ImGui::MenuItem("Save", "CTRL+S") && project.current_scene->save_path != "N/A")
                //{
                    //project.save(project.path);
                //}
                if (ImGui::MenuItem("Save As..", "CTRL+SHIFT+S"))
                {
                    const std::string path = engine::FileDialogs::save_file("Rainfall Project (*.rainp)\0*.rainp\0");
                    //if (!path.empty())
                        //project.save(path);
                }

                ImGui::Separator();

                if (ImGui::BeginMenu("Loaded scenes"))
                {
                    //for (unsigned int i = 0; i < project.scenes.size(); ++i)
                    //{
                    //    if (ImGui::Selectable(project.scenes.at(i)->name.c_str()))
                    //        project.current_scene = project.scenes.at(i).get();
                    //}

                    ImGui::EndMenu();
                }

                ImGui::Separator();

                //if (ImGui::MenuItem("Exit"))
                    //stop();

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
                //ImGui::MenuItem("Show ImGui Demo", "CTRL+D", &show_imgui_demo);

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
}
