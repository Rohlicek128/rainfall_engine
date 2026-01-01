#include "ProjectNewModal.h"

#include <engine/utils/FileDialogs.h>
#include "../../EditorApplication.h"


namespace editor
{
    void ProjectNewModal::draw(EditorApplication& app, bool* open)
    {
        if (*open) ImGui::OpenPopup("New Project");

        ImGui::SetNextWindowSize(ImVec2(500, 0));
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("New Project", open, ImGuiWindowFlags_AlwaysAutoResize))
        {
            //ImGui::PushFont(nullptr, ImGui::GetStyle().FontSizeBase * 2.0f);
            //ImGui::SeparatorText("New Project");
            //ImGui::PopFont();

            ImGui::Text("Create empty project.");
            ImGui::Separator();

            if (ImGui::Button("Create", ImVec2(120, 0)))
            {
                app.reset();
                app.create_sample_scene();

                ImGui::CloseCurrentPopup();
                *open = false;
            }
            ImGui::EndPopup();
        }
    }
}
