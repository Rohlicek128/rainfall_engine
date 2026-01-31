#pragma once

#include <engine/core/Application.h>
#include "RmlUi/Core/ElementDocument.h"


namespace game
{
    class ExampleGame : public engine::Application
    {
        Entity* box_;
        Entity* light_;

        bool show_;
        Rml::String text_;
        Rml::ElementDocument* document_;

    public:
        void on_start() override;
        void on_update(const float delta_time) override;
    };
}
