#include "GeometryProgram.h"

#include "../../Buffers/Textures/Texture.h"
#include "../../Entities/Components/CameraComponent.h"
#include "../../Entities/Components/LightComponent.h"
#include "../../Entities/Components/MaterialComponent.h"
#include "../../Entities/Components/MeshComponent.h"
#include "../../Entities/Components/TextureComponent.h"

GeometryProgram::GeometryProgram(const std::vector<Shader>& shaders) : Program(shaders)
{
}

void GeometryProgram::draw(const Scene& scene, const float aspect_ratio)
{
    bind();
    scene.mesh->bind();
    
    //Camera
    CameraComponent* camera_component = dynamic_cast<CameraComponent*>(scene.current_camera->components->at(CAMERA));
    set_uniform("view", camera_component->get_view_matrix());
    set_uniform("projection", camera_component->get_projection_matrix(aspect_ratio));
    
    set_uniform("material.diffuse_map", 0);
    set_uniform("material.roughness_map", 1);
    set_uniform("material.metallic_map", 2);
    set_uniform("material.normal_map", 3);

    //Entities
    for (const std::unique_ptr<Entity>& entity : scene.entities)
    {
        if (!entity->is_visible || !entity->component_exists(MESH)) continue;

        const glm::mat4 model = entity->get_model_matrix();
        set_uniform("model", model);
        set_uniform("inverse_model", glm::mat3(transpose(inverse(model))));
        
        if (entity->component_exists(TEXTURE))
        {
            TextureComponent* texture = dynamic_cast<TextureComponent*>(entity->components->at(TEXTURE));
            if (texture->type == GL_TEXTURE_2D) texture->active_bind(0);
            set_uniform("material.has_normal_map", texture->has_normal());
            set_uniform("texture_scaling", texture->scale);
        }
        else
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 2);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, 2);
            set_uniform("material.has_normal_map", 0);
            set_uniform("texture_scaling", 1.0f);
        }
        
        if (entity->component_exists(MATERIAL))
        {
            dynamic_cast<MaterialComponent*>(entity->components->at(MATERIAL))->set_uniforms(this);
            if (entity->component_exists(TEXTURE))
            {
                if (dynamic_cast<TextureComponent*>(entity->components->at(TEXTURE))->has_roughness())
                    set_uniform("material.roughness", 0.0f);
                if (dynamic_cast<TextureComponent*>(entity->components->at(TEXTURE))->has_metallic())
                    set_uniform("material.metallic", 0.0f);
            }
        }
        else if (entity->component_exists(LIGHT))
        {
            set_uniform("material.color",
                dynamic_cast<LightComponent*>(entity->components->at(LIGHT))->diffuse *
                    dynamic_cast<LightComponent*>(entity->components->at(LIGHT))->intensity);
            set_uniform("material.roughness", 1.0f);
            set_uniform("material.metallic", 0.0f);
        }
        else
        {
            if (!entity->component_exists(TEXTURE))
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, 3);
                set_uniform("material.roughness", 1.0f);
            }
            else if (dynamic_cast<TextureComponent*>(entity->components->at(TEXTURE))->has_roughness())
                set_uniform("material.roughness", 0.0f);
            set_uniform("material.color", 1.0, 1.0, 1.0);
            set_uniform("material.metallic", 0.0f);
        }

        const MeshComponent* mesh = dynamic_cast<MeshComponent*>(entity->components->at(MESH));
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
