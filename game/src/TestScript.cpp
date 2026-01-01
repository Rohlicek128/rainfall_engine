#include "TestScript.h"
#include <engine/world/Entity.h>

#include <cmath>


namespace game
{
    void TestScript::on_start()
    {
        count_ = 0.0f;
        material_ = self->get_component<MaterialComponent>();
    }

    void TestScript::on_update(const float delta_time)
    {
        count_ += delta_time;
        self->transform->position.x = std::sin(count_) * 3.0f;
        self->transform->position.z = std::cos(count_) * 3.0f;

        if (material_) material_->roughness = std::sin(count_ / 2.0f);
    }
    
    std::string TestScript::get_name()
    {
        return typeid(*this).name();
    }
}
