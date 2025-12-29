#include "ElementInspector.h"

#include "imgui.h"
#include <engine/rendering/Framebuffer.h>


namespace editor
{
    void ElementInspector::draw_framebuffer(Framebuffer& fbo)
    {
        if (fbo.attached_textures.at(0) == nullptr) return;

        ImGui::Image(
            (ImTextureID)(intptr_t)fbo.attached_textures.at(0)->get_handle(),
            {
                (float)fbo.attached_textures.at(0)->get_width(),
                (float)fbo.attached_textures.at(0)->get_height()
            },{0, 1}, {1, 0}
        );
    }
}
