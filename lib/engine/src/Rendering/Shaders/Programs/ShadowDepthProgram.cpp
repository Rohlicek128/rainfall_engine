#include "ShadowDepthProgram.h"
#include "../../Entities/Components/MeshComponent.h"
#include "../../World/Mesh.h"
#include <glad.h>

ShadowDepthProgram::ShadowDepthProgram(const std::vector<Shader>& shaders) : Program(shaders)
{
}

void ShadowDepthProgram::draw(const Scene& scene, const glm::mat4& light_space)
{
    bind();
    scene.mesh->bind();

    set_uniform("light_space", light_space);

    //Entities
    for (const std::unique_ptr<Entity>& entity : scene.entities)
    {
        if (!entity->is_visible || !entity->get_enabled_component<MeshComponent>()) continue;

        set_uniform("model", entity->get_model_matrix());

        const MeshComponent* mesh = entity->get_component<MeshComponent>();
        if (const ModelData* model_data = scene.mesh->get_model(mesh->model_index))
        {
            if (!mesh->is_culled) glDisable(GL_CULL_FACE);
            else if (mesh->is_inverted) glCullFace(GL_FRONT);

            glDrawElements(mesh->primitive_type, model_data->indices_length,
            GL_UNSIGNED_INT, (void*)(scene.mesh->get_model_indices_offset(mesh->model_index) * sizeof(GLuint)));

            if (!mesh->is_culled) glEnable(GL_CULL_FACE);
            else if (mesh->is_inverted) glCullFace(GL_BACK);
        }
    }

    scene.mesh->unbind();
    unbind();
}
