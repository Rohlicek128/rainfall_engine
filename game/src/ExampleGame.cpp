#include "ExampleGame.h"
#include "engine/world/Components/LightComponent.h"
#include "engine/world/Components/MeshComponent.h"
#include "engine/world/Components/TextureComponent.h"
#include "engine/world/Components/CameraComponent.h"
#include "engine/world/components/RidgidbodyComponent.h"
#include "engine/world/components/SphereCollider.h"
#include "glfw3.h"

#include <cmath>
#include <string>
#include <utils.h>

#include "TestScript.h"


namespace game
{
    void ExampleGame::on_start()
    {
        current_project->name = "Example Project";
        current_project->project_dir = "C:\\Files\\Code\\C++\\rainfall_engine\\game\\";


        //scene_manager->load_scene("saved/Example.rain", true);
        //scene_manager->load_scene("saved/Another.rain");

        Scene* scene = scene_manager->create_scene("MyScene", true);
        box_ = scene->create_entity("Sample");
        box_->add_component<MeshComponent>(0, GL_TRIANGLES, resource_manager->get_mesh_manager());
        Texture* texture = resource_manager->load_texture(current_project->project_dir + current_project->assets_dir + "\\chill_guy.jpg", "jpg");
        box_->add_component<TextureComponent>(texture);
        box_->add_component<engine::physics::SphereCollider>();
        box_->add_component<TestScript>();


        light_ = scene->create_entity("Light");
        light_->transform->scale *= 0.2f;
        light_->transform->position.x = 3.0f;
        light_->transform->position.z = 2.0f;
        light_->add_component<MeshComponent>(0, 4, resource_manager->get_mesh_manager());
        light_->add_component<LightComponent>(lights::LIGHT_TYPE::POINT, glm::vec3(1.0f, 1.0f, 0.0f));
        light_->get_component<LightComponent>()->intensity = 5.0f;

        scene->current_camera->add_component<engine::physics::SphereCollider>();

        int model_s = resource_manager->load_model(current_project->project_dir + current_project->assets_dir + "\\models\\sibenik.obj");
        int model_t = resource_manager->load_model(current_project->project_dir + current_project->assets_dir + "\\models\\teapot.obj");
        auto obj = scene->create_entity("Model");
        obj->add_component<MeshComponent>(model_s, GL_TRIANGLES, resource_manager->get_mesh_manager());


        show_ = true;
        text_ = "Something";
        if (Rml::DataModelConstructor constructor = ui->CreateDataModel("testing"))
        {
            constructor.Bind("show", &show_);
            constructor.Bind("text", &text_);
        }

        document_ = ui->LoadDocument("assets/documents/index.rml");
        document_->Show();

        current_project->save(*this, current_project->project_dir + "examplegame.rainp");
        scene->save(current_project->project_dir + current_project->scenes_dir + "\\" + scene->name + ".rain");
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

        if (input_manager->get_key_with_timeout(GLFW_KEY_INSERT, 50))
        {
            Entity* e = scene_manager->get_current_scene()->create_entity("Launched");
            e->transform->position = {std::sin(get_uptime()), 3.0f, std::cos(get_uptime()) * 3.0f};
            e->transform->scale = {0.2f, 0.2f, 0.2f};
            e->add_component<MeshComponent>(0, GL_TRIANGLES, resource_manager->get_mesh_manager());
            e->add_component<RidgidbodyComponent>(*e->transform, 10.0f, glm::vec3(0.0f, 15.0f, 0.0f));
            e->add_component<engine::physics::SphereCollider>();
        }


        Rml::Element* element = document_->GetElementById("mouse");
        element->SetInnerRML("count: <h1>" + std::to_string(scene_manager->get_current_scene()->entities.size()) + "</h1>, " + std::to_string(1.0f / delta_time));
        /*if (input_manager->mouse->get_button_down(GLFW_MOUSE_BUTTON_1))
        {
            element->SetInnerRML("MB1 is Down");
        }
        else element->SetInnerRML("MB1 is Up");*/

        if (input_manager->mouse->get_button_down(GLFW_MOUSE_BUTTON_2))
        {
            //tools::printl_message("EXAMPLE", "MB2 Down");
        }

        if (input_manager->mouse->is_scrolling_y())
        {
            tools::printl_message("EXAMPLE", std::to_string(input_manager->mouse->scroll_y_delta));
        }


        CameraComponent* cur_camera = scene_manager->get_current_scene()->current_camera->get_component<CameraComponent>();
        cur_camera->default_move(*input_manager, delta_time);
        cur_camera->default_mouse_move(*input_manager, delta_time);
    }
}
