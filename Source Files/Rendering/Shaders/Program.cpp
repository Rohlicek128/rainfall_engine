#include "Program.h"

#include <iostream>

#include "../Buffers/Textures/Texture.h"
#include "../Entities/Components/CameraComponent.h"
#include "../Entities/Components/LightComponent.h"
#include "../Entities/Components/MaterialComponent.h"
#include "../Entities/Components/TextureComponent.h"

class MaterialComponent;

Program::Program(const Shader* shaders)
{
    handle_ = glCreateProgram();

    for (int i = 0; i < sizeof(shaders) / sizeof(*shaders); ++i)
    {
        glAttachShader(handle_, shaders[i].get_handle());
    }
    glLinkProgram(handle_);
    
    int success;
    char info_log[512];
    glGetProgramiv(handle_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(handle_, 512, nullptr, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }
    
    for (int i = 0; i < sizeof(shaders) / sizeof(*shaders); ++i)
    {
        glDetachShader(handle_, shaders[i].get_handle());
    }
}
Program::~Program()
{
    glDeleteProgram(handle_);
}

void Program::bind()
{
    glUseProgram(handle_);
}

void Program::unbind()
{
    glUseProgram(0);
}


void Program::draw(Mesh* mesh, const std::vector<std::shared_ptr<Entity>>& lights, const Entity* camera, const GLint* viewport)
{
    bind();
    mesh->bind();

    //Lights
    int dir_index = 0;
    int point_index = 0;
    for (const std::shared_ptr<Entity>& light : lights)
    {
        LightComponent* component = dynamic_cast<LightComponent*>(light->components->at(LIGHT));
        switch (component->type)
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
    CameraComponent* camera_component = dynamic_cast<CameraComponent*>(camera->components->at(CAMERA));
    set_uniform("view_pos", camera->transform->position);
    
    set_uniform("material.diffuse_map", 0);
    set_uniform("material.specular_map", 1);

    //Entities
    int offset = 0;
    for (std::shared_ptr<Entity>& entity : mesh->entities)
    {
        if (entity->mesh == nullptr) continue;
        if (!entity->is_visible)
        {
            offset += entity->mesh->inds_lenght; 
            continue;
        }
        
        set_uniform("model", entity->transform->get_model_matrix());
        set_uniform("inverse_model", glm::mat3(transpose(inverse(entity->transform->get_model_matrix()))));
        set_uniform("view", camera_component->get_view_matrix());
        set_uniform("projection", camera_component->get_projection_matrix((float)viewport[0] / (float)viewport[1]));
        
        if (entity->component_exists(TEXTURE))
            dynamic_cast<TextureComponent*>(entity->components->at(TEXTURE))->active_bind(0);
        else
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 1);
        }
        
        if (entity->component_exists(MATERIAL))
        {
            dynamic_cast<MaterialComponent*>(entity->components->at(MATERIAL))->set_uniforms(this);
        }
        else if (entity->component_exists(LIGHT))
        {
            set_uniform("material.color",
                dynamic_cast<LightComponent*>(entity->components->at(LIGHT))->diffuse *
                    dynamic_cast<LightComponent*>(entity->components->at(LIGHT))->intensity);
            set_uniform("material.shininess", 32.0f);
            set_uniform("is_light", 1);
        }
        else
        {
            if (!entity->component_exists(TEXTURE))
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, 3);
            }
            set_uniform("material.color", 1.0, 1.0, 1.0);
            set_uniform("material.shininess", 32.0f);
            set_uniform("is_light", 0);
        }
        
        glDrawElements(entity->mesh->primitive_type, entity->mesh->inds_lenght, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
        offset += entity->mesh->inds_lenght; 
    }

    mesh->unbind();
    unbind();
}

void Program::set_uniform(const char* name, const int value)
{
    const int location = glGetUniformLocation(handle_, name);
    if (location == -1) return;
    glUniform1i(location, value);
}


void Program::set_uniform(const char* name, const float value)
{
    const int location = glGetUniformLocation(handle_, name);
    if (location == -1) return;
    glUniform1f(location, value);
}

void Program::set_uniform(const char* name, const glm::vec3& value)
{
    const int location = glGetUniformLocation(handle_, name);
    if (location == -1) return;
    glUniform3f(location, value.x, value.y, value.z);
}

void Program::set_uniform(const char* name, const float v0, const float v1, const float v2)
{
    const int location = glGetUniformLocation(handle_, name);
    if (location == -1) return;
    glUniform3f(location, v0, v1, v2);
}

void Program::set_uniform(const char* name, const glm::mat3& value)
{
    const int location = glGetUniformLocation(handle_, name);
    if (location == -1) return;
    glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

void Program::set_uniform(const char* name, const glm::vec4& value)
{
    const int location = glGetUniformLocation(handle_, name);
    if (location == -1) return;
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Program::set_uniform(const char* name, const glm::mat4& value)
{
    const int location = glGetUniformLocation(handle_, name);
    if (location == -1) return;
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}