#pragma once

#define RMLUI_STATIC_LIB
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>


class SystemInterface_GLFW;
class RenderInterface_GL3;

namespace engine
{
    class RmlUiLayer
    {
        std::unique_ptr<SystemInterface_GLFW> system_interface_;
        std::unique_ptr<RenderInterface_GL3> render_interface_;
        Rml::Context* context_;

    public:
        RmlUiLayer(int width, int height);
        ~RmlUiLayer();

        void update();
        void render(int width, int height);
    };
}
