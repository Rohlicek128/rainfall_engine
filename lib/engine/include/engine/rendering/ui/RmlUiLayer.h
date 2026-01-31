#pragma once

#include "RmlUi/Core/Context.h"
#include "engine/managers/InputManager.h"
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>


class SystemInterface_GLFW;
class RenderInterface_GL3;

namespace engine
{
    class InputManager;

    class RmlUiLayer
    {
        std::unique_ptr<SystemInterface_GLFW> system_interface_;
        std::unique_ptr<RenderInterface_GL3> render_interface_;
        Rml::Context* context_;

        bool debug_;

    public:
        RmlUiLayer(int width, int height, bool debug = false);
        ~RmlUiLayer();

        void update(InputManager& input);
        void render(int width, int height);

        Rml::Context* get_context();
    };
}
