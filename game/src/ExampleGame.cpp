#include "ExampleGame.h"
#include "engine/world/Components/LightComponent.h"
#include "engine/world/Components/MeshComponent.h"
#include "engine/world/Components/TextureComponent.h"
#include "engine/world/Components/CameraComponent.h"
#include "engine/world/components/MaterialComponent.h"
#include "engine/world/components/RidgidbodyComponent.h"
#include "engine/world/components/SphereCollider.h"
#include "glfw3.h"

#include <cmath>
#include <string>
#include <utils.h>

#include "scripts/BulletScript.h"
#include "scripts/CameraScript.h"
#include "scripts/EnemyScript.h"

#include <random>


namespace game
{
    void ExampleGame::on_start()
    {
        current_project->load(*this, "sr_game.rainp");

        std::srand(time(0));

        Scene* scene = scene_manager->get_current_scene();

        scene->current_camera = scene->get_entity("Player");
        scene->current_camera->add_component<engine::physics::SphereCollider>();
        scene->current_camera->add_component<CameraScript>();
        scene->current_camera->get_component<CameraScript>()->set_input_manager(input_manager.get());
        scene->current_camera->transform->position.y = 1.0f;

        firerate_ = 0.1f;
        firerate_count_ = 0.0f;
        spawnrate_ = 0.5f;
        spawnrate_count_ = 0.0f;

        sphere_model_ = resource_manager->load_model("assets\\models\\sphere.obj");


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
        Rml::Element* element = document_->GetElementById("mouse");
        element->SetInnerRML("count: <h1>" +
            std::to_string(scene_manager->get_current_scene()->current_camera->get_component<CameraScript>()->health) + "</h1>, " +
            std::to_string(scene_manager->get_current_scene()->entities.size()));

        spawnrate_count_ += delta_time;
        if (spawnrate_count_ >= spawnrate_)
        {
            spawnrate_count_ = 0.0f;

            Entity* e = scene_manager->get_current_scene()->create_entity("Enemy");
            e->transform->position = {
                (float)(std::rand()) / (float)(RAND_MAX) * 50.0f - 25.0f,
                0.0f,
                (float)(std::rand()) / (float)(RAND_MAX) * 50.0f - 25.0f
            };
            e->transform->scale = {0.7f, 0.7f, 0.7f};
            e->add_component<MeshComponent>(0);
            e->add_component<MaterialComponent>(glm::vec4(1.0));
            e->add_component<engine::physics::SphereCollider>();
            e->add_component<EnemyScript>();
            e->get_component<EnemyScript>()->scene_manager = scene_manager.get();
            e->get_component<EnemyScript>()->target = scene_manager->get_current_scene()->current_camera->transform;
        }


        firerate_count_ += delta_time;
        if (firerate_count_ >= firerate_)
            firerate_count_ = 0.0f;

        if (input_manager->mouse->get_button_down(GLFW_MOUSE_BUTTON_1) && firerate_count_ <= 0.0f)
        {
            Entity* e = scene_manager->get_current_scene()->create_entity("Bullet");
            e->transform->position = scene_manager->get_current_scene()->current_camera->transform->position;
            e->transform->scale = {0.1f, 0.1f, 0.1f};
            e->add_component<MeshComponent>(0);
            e->add_component<engine::physics::SphereCollider>();
            e->add_component<BulletScript>();
            e->get_component<BulletScript>()->set_forward(scene_manager->get_current_scene()->current_camera->transform->rotation);

            bullets_.push(e);
        }

        if (bullets_.size() >= 50)
        {
            scene_manager->get_current_scene()->remove_entity(bullets_.front());
            bullets_.pop();
        }
    }
}
