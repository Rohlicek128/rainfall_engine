#include "PerformancePanel.h"

#include <engine/rendering/Renderer.h>


namespace editor
{
    void PerformancePanel::draw(ImVec2 pos, engine::Renderer& renderer)
    {
        ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.35f);
        if (ImGui::Begin("Performance", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
        {
            ImGui::Text("%i fps (%.4f ms, Max: %i)", renderer.display_frame_count, renderer.delta_time * 1000.0f, renderer.max_fps_plot);
            if (ImGui::TreeNode("FPSHistogram", "FPS Histogram (past %i s)", std::size(renderer.fps_plot)))
            {
                ImGui::PlotHistogram("##FPSHistory", renderer.fps_plot, std::size(renderer.fps_plot), 0, nullptr, 0, (float)renderer.max_fps_plot, {0, 80});
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Stats"))
            {
                ImGui::Text("Uptime: %.3f s", glfwGetTime());
                //ImGui::Text("Window W: %i, H: %i", args.width, args.height);
                ImGui::TreePop();
            }
        }
        ImGui::End();
    }
}
