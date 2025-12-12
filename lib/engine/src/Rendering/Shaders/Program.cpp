#include "Program.h"

#include <iostream>

#include <glad.h>


class MaterialComponent;

Program::Program(const std::vector<Shader>& shaders)
{
    handle_ = glCreateProgram();

    for (int i = 0; i < shaders.size(); ++i)
        glAttachShader(handle_, shaders[i].get_handle());
    glLinkProgram(handle_);

    is_successful();

    for (int i = 0; i < shaders.size(); ++i)
        glDetachShader(handle_, shaders[i].get_handle());
}
Program::~Program()
{
    glDeleteProgram(handle_);
}

void Program::is_successful()
{
    int success;
    char info_log[512];
    glGetProgramiv(handle_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(handle_, std::size(info_log), nullptr, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }
}

void Program::bind()
{
    glUseProgram(handle_);
}

void Program::unbind()
{
    glUseProgram(0);
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
