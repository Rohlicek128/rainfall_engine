#include "engine/rendering/ui/RmlUiLayer.h"
#include "RmlUi/Core/Core.h"

#include <RmlUi/Backends/RmlUi_Platform_GLFW.h>
#include <RmlUi/Backends/RmlUi_Renderer_GL3.h>

#include <memory>
#include <iostream>


namespace engine
{
    RmlUiLayer::RmlUiLayer(int width, int height)
    {
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
        Rml::Debugger::SetVisible(true);

        if (!Rml::LoadFontFace("engine/assets/fonts/Lexend-Regular.ttf"))
        {
            std::cerr << "Could not load font! Check your working directory." << std::endl;
        }

        Rml::ElementDocument* document = context_->LoadDocument("engine/assets/documents/index.rml");
        if (document)
        {
            document->Show();
        }
        else
        {
            std::cerr << "Could not load document!" << std::endl;
        }
    }

    RmlUiLayer::~RmlUiLayer()
    {
        Rml::Shutdown();
    }


    void RmlUiLayer::update()
    {

    }

    void RmlUiLayer::render(int width, int height)
    {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_SCISSOR_TEST);

        render_interface_->SetViewport(width, height);

        context_->Update();
        context_->Render();

        glEnable(GL_DEPTH_TEST);
    }
}
