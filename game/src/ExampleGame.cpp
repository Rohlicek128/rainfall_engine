#include "ExampleGame.h"

#include <utils.h>


namespace game
{
    void ExampleGame::on_start()
    {
        count = 0;
        scene_manager->load_scene("saved/Example.rain", true);
        //scene_manager->load_scene("saved/Another.rain");

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
