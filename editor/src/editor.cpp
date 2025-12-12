#include "engine/core/Engine.h"
#include "engine/rendering/Window.h"
#include "engine/src/Rendering/Entities/Components/MeshComponent.h"
#include <iostream>
#include <engine/core/Application.h>

class e_test : public engine::Application
{
public:
    void on_start() override
    {
        scene_manager->create_scene("editor", true);
        auto e = scene_manager->get_current_scene()->create_entity("Cube");
        e->add_component<MeshComponent>(0, GL_TRIANGLES, resource_manager->get_mesh_manager());
    }

    void on_update(const float delta_time) override
    {

    }
};


int main()
{
    std::cout << "Hello from Editor\n";

    engine::Run<e_test>();

    return 0;
}
