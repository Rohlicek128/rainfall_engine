#pragma once
#include <string>

#include "../IBindable.h"

#include <glad/glad.h>

class Texture : public IBindable
{
    std::string path_;
    int width_, height_, nr_channels_;
    GLenum internal_format_, format_, type_;
    
    unsigned char* load_image(const std::string&);
public:
    Texture(const std::string&, GLenum = GL_RGB, GLenum = GL_UNSIGNED_BYTE);
    Texture(int, int, GLenum = GL_RGB8, GLenum = GL_RGB, GLenum = GL_UNSIGNED_BYTE);
    ~Texture() override;
    std::string& get_path();
    int get_width();
    int get_height();
    int get_nr_channels();
    
    void active_bind(unsigned int);
    void resize(int, int);
    void bind() override;
    void unbind() override;
};
