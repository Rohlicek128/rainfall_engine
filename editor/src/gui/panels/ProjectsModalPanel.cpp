#include "ProjectsModalPanel.h"

#include <engine/utils/FileDialogs.h>
#include <engine/core/Application.h>


namespace editor
{
    void ProjectsModalPanel::draw(engine::Application& app, bool* open, bool* open_new)
    {
        if (*open) ImGui::OpenPopup("Open Project");

        ImGui::SetNextWindowSize(ImVec2(600, 350));
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Open Project", open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
        {
            ImGui::PushFont(nullptr, ImGui::GetStyle().FontSizeBase * 2.0f);
            ImGui::SeparatorText("RAINFALL EDITOR");
            ImGui::PopFont();

            ImGui::Text("Choose which project you want to open:");
            ImGui::Separator();

            if (ImGui::Button("New", ImVec2(120, 0)))
            {
                *open_new = true;

                ImGui::CloseCurrentPopup();
                *open = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Open...", ImVec2(120, 0)))
            {
                const std::string path = engine::FileDialogs::open_file("Rainfall Project (*.rainp)\0*.rainp\0");
                if (!path.empty())
                    app.current_project->load(app, path);

                ImGui::CloseCurrentPopup();
                *open = false;
            }

            if (ImGui::Button("Close", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
                *open = false;
            }
            ImGui::EndPopup();
        }
    }
}
