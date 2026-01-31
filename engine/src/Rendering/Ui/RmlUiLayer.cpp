#include "engine/rendering/ui/RmlUiLayer.h"
#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/Core.h"
#include "RmlUi/Core/Math.h"
#include "RmlUi/Debugger/Debugger.h"
#include "glfw3.h"

#include <RmlUi/Backends/RmlUi_Platform_GLFW.h>
#include <RmlUi/Backends/RmlUi_Renderer_GL3.h>

#include <memory>
#include <iostream>


namespace engine
{
    RmlUiLayer::RmlUiLayer(int width, int height, bool debug)
    {
        debug_ = debug;

        system_interface_ = std::make_unique<SystemInterface_GLFW>();
        render_interface_ = std::make_unique<RenderInterface_GL3>();

        Rml::SetSystemInterface(system_interface_.get());
        Rml::SetRenderInterface(render_interface_.get());

        if (!Rml::Initialise())
        {
            std::cerr << "RmlUi failed to initialize!" << std::endl;
        }


        context_ = Rml::CreateContext("main", Rml::Vector2i(width, height));

        Rml::Debugger::Initialise(context_);
        Rml::Debugger::SetVisible(false);

        if (!Rml::LoadFontFace("engine/assets/fonts/Lexend-Regular.ttf"))
        {
            std::cerr << "Could not load font! Check your working directory." << std::endl;
        }
    }

    RmlUiLayer::~RmlUiLayer()
    {
        Rml::Shutdown();
    }

    Rml::Context* RmlUiLayer::get_context()
    {
        return context_;
    }


    void RmlUiLayer::update(InputManager& input)
    {
        if (input.mouse->moved())
            context_->ProcessMouseMove(input.mouse->pos_x, input.mouse->pos_y, 0);

        if (input.mouse->get_button_down(GLFW_MOUSE_BUTTON_1)) context_->ProcessMouseButtonDown(0, 0);
        else context_->ProcessMouseButtonUp(0, 0);

        if (input.mouse->get_button_down(GLFW_MOUSE_BUTTON_2)) context_->ProcessMouseButtonDown(1, 0);
        else context_->ProcessMouseButtonUp(1, 0);

        if (input.mouse->get_button_down(GLFW_MOUSE_BUTTON_3)) context_->ProcessMouseButtonDown(2, 0);
        else context_->ProcessMouseButtonUp(2, 0);

        if (input.mouse->is_scrolling_x() || input.mouse->is_scrolling_y())
            context_->ProcessMouseWheel(Rml::Vector2f(-input.mouse->scroll_x_delta, -input.mouse->scroll_y_delta), 0);


        if (debug_ && input.get_key_toggle(GLFW_KEY_F8))
            Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
    }

    void RmlUiLayer::render(int width, int height)
    {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        render_interface_->SetViewport(width, height);

        context_->Update();
        context_->Render();

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }
}
