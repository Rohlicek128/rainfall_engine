#include "ViewportPanel.h"

#include "../inspectors/ElementInspector.h"


namespace editor
{
    void ViewportPanel::draw(Framebuffer& fbo)
    {
        ImGui::Begin("Viewport", nullptr);

        //pos  = ImGui::GetCursorScreenPos();
        size = ImGui::GetContentRegionAvail();
        //resize((int)size_vp.x, (int)size_vp.y);

        ElementInspector::draw_framebuffer(fbo);

        pos = ImGui::GetWindowPos();
        pos.x += ImGui::GetWindowContentRegionMin().x + 10;
        pos.y += ImGui::GetWindowContentRegionMin().y + 10;

        ImGui::End();
    }
}
