#include "ExampleGame.h"
#include "engine/rendering/Window.h"
#include "engine/src/Rendering/Entities/Components/MeshComponent.h"

#include <utils.h>


namespace game
{
    void ExampleGame::on_start()
    {
        count = 0;
        //scene_manager->load_scene("saved/Example.rain", true);
        //scene_manager->load_scene("saved/Another.rain");

        Scene* scene = scene_manager->create_scene("MyScene", true);
        Entity* e = scene->create_entity("Sample");
        e->add_component<MeshComponent>(0, GL_TRIANGLES, resource_manager->get_mesh_manager());

        resource_manager->load_texture("assets/chill_guy.jpg", "jpg");


        tools::printl_message("EXAMPLE", "OnStart");
    }

    void ExampleGame::on_update(const float delta_time)
    {
        /*count += delta_time;

        if (count >= 5.0f)
        {
            //scene_manager->switch_to("Another");
            count = 0;
            tools::printl_message("EXAMPLE", "Scene switched");
        }*/
    }
}
