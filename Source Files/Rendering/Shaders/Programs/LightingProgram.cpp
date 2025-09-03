#include "LightingProgram.h"

#include "../../Buffers/Textures/Texture.h"
#include "../../Entities/Components/CameraComponent.h"
#include "../../Entities/Components/LightComponent.h"
#include "../../Entities/Components/MaterialComponent.h"


LightingProgram::LightingProgram(const std::vector<Shader>& shaders) : Program(shaders)
{
}

void LightingProgram::draw(const Scene& scene, Mesh& screen_mesh, const int quad_index, GBuffer& g_buffer)
{
    glDisable(GL_DEPTH_TEST);
    bind();
    screen_mesh.bind();

    //Lights
    int dir_index = 0;
    int point_index = 0;
    for (const Entity* light : scene.lights)
    {
        switch (LightComponent* component = dynamic_cast<LightComponent*>(light->components->at(LIGHT)); component->type)
        {
            case DIRECTIONAL:
                component->set_uniforms(this, dir_index, light->transform);
                dir_index++;
                break;
            case POINT:
                component->set_uniforms(this, point_index, light->transform);
                point_index++;
                break;
        }
    }
    
    //Camera
    set_uniform("view_pos", scene.current_camera->transform->position);

    //GBuffer
    g_buffer.active_bind();
    set_uniform("g_position", 0);
    set_uniform("g_albedo_spec", 1);
    set_uniform("g_normal_shin", 2);
    
    set_uniform("is_shaded", 1);

    if (const ModelData* model_data = screen_mesh.get_model(quad_index))
    {
        glDrawElements(GL_TRIANGLES, model_data->indices_length,
            GL_UNSIGNED_INT, (void*)(screen_mesh.get_model_indices_offset(quad_index) * sizeof(GLuint)));
    }

    screen_mesh.unbind();
    unbind();
    glEnable(GL_DEPTH_TEST);
}
