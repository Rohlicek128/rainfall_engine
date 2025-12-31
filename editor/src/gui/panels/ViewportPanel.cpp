#include "ViewportPanel.h"
#include <engine/rendering/Renderer.h>

#include "../inspectors/ElementInspector.h"


namespace editor
{
    void ViewportPanel::draw(engine::Renderer& renderer)
    {
        ImGui::Begin("Viewport", nullptr);

        size = ImGui::GetContentRegionAvail();
        renderer.resize((int)size.x, (int)size.y);

        ElementInspector::draw_framebuffer(*renderer.get_render_fbo());

        pos = ImGui::GetWindowPos();
        pos.x += ImGui::GetWindowContentRegionMin().x + 10;
        pos.y += ImGui::GetWindowContentRegionMin().y + 10;

        ImGui::End();
    }
}
