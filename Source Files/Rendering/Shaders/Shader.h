#pragma once
#include <string>

#include "glad/glad.h"

class Shader
{
    unsigned int handle_;
    std::string read_file(const char*) const;
public:
    Shader(const std::string&, GLenum);
    ~Shader();
    unsigned int get_handle() const;
};
