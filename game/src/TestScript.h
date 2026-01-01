#pragma once

#include "engine/world/components/MaterialComponent.h"
#include <engine/world/components/BehaviorComponent.h>


namespace game
{
    class TestScript : public BehaviorComponent
    {
        float count_;
        MaterialComponent* material_;

    public:
        void on_start() override;
        void on_update(const float delta_time) override;
        
        std::string get_name() override;
    };
}
