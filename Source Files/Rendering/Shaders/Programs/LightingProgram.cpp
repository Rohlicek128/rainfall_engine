#include "LightingProgram.h"

#include "../../Buffers/Textures/Texture.h"
#include "../../Entities/Components/CameraComponent.h"
#include "../../Entities/Components/LightComponent.h"
#include "../../Entities/Components/MaterialComponent.h"

LightingProgram::LightingProgram(const std::vector<Shader>& shaders) : Program(shaders)
{
    ambient = glm::vec3(0.01f);
}

void LightingProgram::draw(const Scene& scene, Mesh& screen_mesh, const int quad_index, GBuffer& g_buffer)
{
    glDisable(GL_DEPTH_TEST);
    bind();
    screen_mesh.bind();

    //Lights
    set_lights_uniforms(scene, DIRECTIONAL, nr_directional_lights);
    set_lights_uniforms(scene, POINT, nr_point_lights);
    
    //Camera
    set_uniform("view_pos", scene.current_camera->transform->position);
    set_uniform("ambient", ambient);

    //GBuffer
    g_buffer.active_bind();
    set_uniform("g_position", 0);
    set_uniform("g_albedo_rough", 1);
    set_uniform("g_normal_metal", 2);

    if (const ModelData* model_data = screen_mesh.get_model(quad_index))
    {
        glDrawElements(GL_TRIANGLES, model_data->indices_length,
            GL_UNSIGNED_INT, (void*)(screen_mesh.get_model_indices_offset(quad_index) * sizeof(GLuint)));
    }

    screen_mesh.unbind();
    unbind();
    glEnable(GL_DEPTH_TEST);
}

void LightingProgram::set_lights_uniforms(const Scene& scene, const LIGHT_TYPE type, const unsigned int type_max)
{
    unsigned int light_count = 0;
    for (unsigned int i = 0; i < scene.lights.size(); ++i)
    {
        if (light_count >= type_max) break;
        if (LightComponent* component = scene.lights.at(i)->get_component<LightComponent>())
        {
            if (component->type == type)
            {
                component->set_uniforms(this, light_count, scene.lights.at(i)->transform);
                light_count++;
            }
        }
    }

    for (unsigned int i = light_count; i < type_max; ++i)
    {
        switch (type)
        {
            case DIRECTIONAL: set_uniform(("dir_lights[" + std::to_string(i) + "].is_lighting").c_str(), false); break;
            case POINT: set_uniform(("point_lights[" + std::to_string(i) + "].is_lighting").c_str(), false); break;
            case SPOTLIGHT: break;
        }
    }
}