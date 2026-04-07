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
        current_project->load(*this, "C:\\Files\\Code\\C++\\rainfall_engine\\game\\sr_game.rainp");

        Scene* scene = scene_manager->get_current_scene();
        box_ = scene->get_entity("Cube");
        box_->add_component<engine::physics::SphereCollider>();
        box_->add_component<TestScript>();

        light_ = scene->get_entity("Sun");

        scene->current_camera = scene->get_entity("Player");
        scene->current_camera->add_component<engine::physics::SphereCollider>();

        //int model_s = resource_manager->load_model(current_project->project_dir + current_project->assets_dir + "\\models\\sibenik.obj");
        //int model_t = resource_manager->load_model(current_project->project_dir + current_project->assets_dir + "\\models\\sphere.obj");
        //auto obj = scene->create_entity("Model");
        //obj->add_component<MeshComponent>(model_t, GL_TRIANGLES, resource_manager->get_mesh_manager());
        //obj->transform->scale *= 3.0f;


        show_ = true;
        text_ = "Something";
        if (Rml::DataModelConstructor constructor = ui->CreateDataModel("testing"))
        {
            constructor.Bind("show", &show_);
            constructor.Bind("text", &text_);
        }

        document_ = ui->LoadDocument("assets/documents/index.rml");
        document_->Show();

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
            e->add_component<MeshComponent>(0);
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
