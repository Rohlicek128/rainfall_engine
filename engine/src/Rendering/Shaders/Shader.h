#pragma once
#include <string>

class Shader
{
    unsigned int handle_;
    std::string read_file(const char*);
public:
    Shader(const std::string&, int);
    ~Shader();
    unsigned int get_handle() const;
};
