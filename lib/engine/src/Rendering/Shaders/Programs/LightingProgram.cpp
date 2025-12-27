#include "LightingProgram.h"

#include "engine/rendering/Texture.h"
#include "engine/world/Entity.h"
#include "engine/world/Components/CameraComponent.h"
#include "engine/world/Components/LightComponent.h"
#include "engine/world/Components/MaterialComponent.h"
#include "engine/managers/Mesh.h"
#include "../Shadows/ShadowMap.h"

LightingProgram::LightingProgram(const std::vector<Shader>& shaders) : Program(shaders)
{
    ambient = glm::vec3(0.01f);
    ambient_edit_ = new float[] {ambient.x, ambient.y, ambient.z};

    is_fog_enabled_ = true;
    fog_color_ = glm::vec3(1.0f);
    fog_color_edit_ = new float[] {fog_color_.x, fog_color_.y, fog_color_.z};
    fog_end_ = 90.0f;
    fog_density_ = 0.2f;
}

void LightingProgram::draw(const Scene& scene, Mesh& screen_mesh, const int quad_index, GBuffer& g_buffer, ShadowMap& shadow_map)
{
    glDisable(GL_DEPTH_TEST);
    bind();
    screen_mesh.bind();

    //Lights
    set_lights_uniforms(scene, lights::LIGHT_TYPE::DIRECTIONAL, nr_directional_lights);
    set_lights_uniforms(scene, lights::LIGHT_TYPE::POINT, nr_point_lights);

    //Camera
    set_uniform("view_pos", scene.current_camera->transform->position);
    set_uniform("ambient", ambient);

    //GBuffer
    g_buffer.active_bind();
    set_uniform("g_position", 0);
    set_uniform("g_albedo_rough", 1);
    set_uniform("g_normal_metal", 2);

    //Shadow
    shadow_map.get_depth_map()->attached_textures.at(0)->active_bind(3);
    set_uniform("shadow_map", 3);
    set_uniform("light_space", shadow_map.light_space);
    set_uniform("is_shadow", shadow_map.is_visible);

    //Fog
    set_uniform("fog.is_enabled", is_fog_enabled_);
    set_uniform("fog.color", fog_color_);
    set_uniform("fog.end", fog_end_);
    set_uniform("fog.density", fog_density_);

    if (const ModelData* model_data = screen_mesh.get_model(quad_index))
    {
        glDrawElements(GL_TRIANGLES, model_data->indices_length,
            GL_UNSIGNED_INT, (void*)(screen_mesh.get_model_indices_offset(quad_index) * sizeof(GLuint)));
    }

    screen_mesh.unbind();
    unbind();
    glEnable(GL_DEPTH_TEST);
}

void LightingProgram::set_lights_uniforms(const Scene& scene, const lights::LIGHT_TYPE type, const unsigned int type_max)
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
            case lights::LIGHT_TYPE::DIRECTIONAL: set_uniform(("dir_lights[" + std::to_string(i) + "].is_lighting").c_str(), false); break;
            case lights::LIGHT_TYPE::POINT: set_uniform(("point_lights[" + std::to_string(i) + "].is_lighting").c_str(), false); break;
            case lights::LIGHT_TYPE::SPOTLIGHT: break;
        }
    }
}
