#pragma once

#include <engine/core/Application.h>


namespace game
{
    class ExampleGame : public engine::Application
    {
        Entity* box_;
        Entity* light_;

    public:
        void on_start() override;
        void on_update(const float delta_time) override;
    };
}
