#include "SkyboxProgram.h"

#include "../../Buffers/Textures/Texture.h"
#include "../../Entities/Components/CameraComponent.h"
#include "../../Entities/Components/LightComponent.h"
#include "../../Entities/Components/MaterialComponent.h"
#include "../../Entities/Components/MeshComponent.h"
#include "../../Entities/Components/TextureComponent.h"
#include "engine/managers/Mesh.h"
#include "engine/world/Entity.h"


SkyboxProgram::SkyboxProgram(const std::vector<Shader>& shaders) : Program(shaders)
{
}

void SkyboxProgram::draw(const Scene& scene, const float aspect_ratio)
{
    if (!scene.skybox->get_enabled_component<MeshComponent>() || !scene.skybox->get_enabled_component<TextureComponent>()) return;

    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    bind();
    scene.mesh->bind();

    CameraComponent* camera_component = scene.current_camera->get_component<CameraComponent>();
    set_uniform("view", glm::mat4(glm::mat3(camera_component->get_view_matrix())));
    set_uniform("projection", camera_component->get_projection_matrix(aspect_ratio));

    scene.skybox->get_component<TextureComponent>()->active_bind(0);
    set_uniform("skybox", 0);

    float intensity = 0.0f;
    for (int i = 0; i < scene.lights.size(); ++i)
    {
        if (const LightComponent* light = scene.lights.at(i)->get_component<LightComponent>())
        {
            if (light->type == lights::LIGHT_TYPE::DIRECTIONAL)
                intensity += light->intensity;
        }
    }
    set_uniform("sun_intensity", intensity / 2.0f);


    const int model_index = scene.skybox->get_component<MeshComponent>()->model_index;
    if (const ModelData* model_data = scene.mesh->get_model(model_index))
    {
        glDrawElements(GL_TRIANGLES, model_data->indices_length,
            GL_UNSIGNED_INT, (void*)(scene.mesh->get_model_indices_offset(model_index) * sizeof(GLuint)));
    }

    scene.mesh->unbind();
    unbind();
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
}
