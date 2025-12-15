#pragma once

#include <engine/core/Application.h>
#include <string>

namespace editor
{
    class EditorEngine : public engine::Application
    {
        std::string working_dir_;

    public:
        EditorEngine();

        void on_start() override;
        void on_update(const float delta_time) override;
    };
}
