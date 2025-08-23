#include "Shader.h"

#include <fstream>
#include <iostream>

std::string Shader::read_file(const char* path) const
{
    std::ifstream file(path);
    std::string file_contents {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
    return file_contents;
}

Shader::Shader(const char* path, GLenum type)
{
    handle_ = glCreateShader(type);

    const std::string str = read_file(path);
    const char* source = str.c_str();
    glShaderSource(handle_, 1, &source, nullptr);
    glCompileShader(handle_);

    int success;
    char infoLog[512];
    glGetShaderiv(handle_, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(handle_, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
    }
}
Shader::~Shader()
{
    glDeleteShader(handle_);
}

unsigned int Shader::get_handle() const
{
    return handle_;
}
