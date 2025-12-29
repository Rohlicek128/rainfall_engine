#include "ExampleGame.h"
#include "engine/world/Components/LightComponent.h"
#include "engine/world/Components/MeshComponent.h"
#include "engine/world/Components/TextureComponent.h"
#include "glfw3.h"

#include <cmath>
#include <utils.h>

#include "TestScript.h"


namespace game
{
    void ExampleGame::on_start()
    {
        //scene_manager->load_scene("saved/Example.rain", true);
        //scene_manager->load_scene("saved/Another.rain");

        Scene* scene = scene_manager->create_scene("MyScene", true);
        box_ = scene->create_entity("Sample");
        box_->add_component<MeshComponent>(0, GL_TRIANGLES, resource_manager->get_mesh_manager());

        Texture* texture = resource_manager->load_texture("assets/chill_guy.jpg", "jpg");
        box_->add_component<TextureComponent>(texture);
        box_->add_component<TestScript>();


        light_ = scene->create_entity("Light");
        light_->transform->scale *= 0.2f;
        light_->transform->position.x = 3.0f;
        light_->transform->position.z = 2.0f;
        light_->add_component<MeshComponent>(0, 4, resource_manager->get_mesh_manager());
        light_->add_component<LightComponent>(lights::LIGHT_TYPE::POINT, glm::vec3(1.0f, 1.0f, 0.0f));
        light_->get_component<LightComponent>()->intensity = 5.0f;
        scene->add_light(light_);


        tools::printl_message("EXAMPLE", "OnStart");
    }

    void ExampleGame::on_update(const float delta_time)
    {
        light_->transform->position.y = std::sin(get_uptime()) * 3;

        if (input_manager->get_key_with_timeout(GLFW_KEY_R, 100))
        {
            box_->transform->position.x += 0.5f;
            //tools::printl_message("EXAMPLE", "R");
        }
        if (input_manager->get_key_toggle(GLFW_KEY_T))
        {
            box_->transform->position.x -= 1.0f;
            tools::printl_message("EXAMPLE", "T");
        }
        if (input_manager->get_key_down(GLFW_KEY_Y))
        {
            box_->transform->position.x -= delta_time;
        }
    }
}
