#include "EditorEngine.h"

#include "engine/src/Rendering/Entities/Components/MeshComponent.h"


namespace editor
{
    EditorEngine::EditorEngine()
    {
        working_dir_ = "C:\\Files\\Code\\C++\\rainfall_engine\\game\\";
    }

    void EditorEngine::on_start()
    {
        scene_manager->create_scene("editor", true);
        scene_manager->load_scene(working_dir_ + "saved\\Another.rain", true);

        //auto e = scene_manager->get_current_scene()->create_entity("Cube");
        //e->add_component<MeshComponent>(0, GL_TRIANGLES, resource_manager->get_mesh_manager());
    }

    void EditorEngine::on_update(const float delta_time)
    {

    }
}
