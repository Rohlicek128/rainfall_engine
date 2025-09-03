#include "SkyboxProgram.h"

#include "../../Buffers/Textures/Texture.h"
#include "../../Entities/Components/CameraComponent.h"
#include "../../Entities/Components/LightComponent.h"
#include "../../Entities/Components/MaterialComponent.h"
#include "../../Entities/Components/MeshComponent.h"
#include "../../Entities/Components/TextureComponent.h"

SkyboxProgram::SkyboxProgram(const std::vector<Shader>& shaders) : Program(shaders)
{
}

void SkyboxProgram::draw(const Scene& scene, const float aspect_ratio)
{
    if (!scene.skybox->component_exists(MESH) || !scene.skybox->component_exists(TEXTURE)) return;

    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    bind();
    scene.mesh->bind();

    CameraComponent* camera_component = dynamic_cast<CameraComponent*>(scene.current_camera->components->at(CAMERA));
    set_uniform("view", glm::mat4(glm::mat3(camera_component->get_view_matrix())));
    set_uniform("projection", camera_component->get_projection_matrix(aspect_ratio));

    dynamic_cast<TextureComponent*>(scene.skybox->components->at(TEXTURE))->active_bind(0);
    set_uniform("skybox", 0);

    float intensity = 0.0f;
    for (int i = 0; i < scene.lights.size(); ++i)
    {
        if (scene.lights.at(i)->component_exists(LIGHT))
        {
            if (const LightComponent* light = dynamic_cast<LightComponent*>(scene.lights.at(i)->components->at(LIGHT));
                light->type == DIRECTIONAL) intensity += light->intensity;
        }
    }
    set_uniform("sun_intensity", intensity / 2.0f);

    
    const int model_index = dynamic_cast<MeshComponent*>(scene.skybox->components->at(MESH))->model_index;
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
