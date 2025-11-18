#pragma once
#include <string>

#include "../IBindable.h"

#include <glad.h>

class Texture : public IBindable
{
    std::string path_;
    int width_, height_, nr_channels_;

    unsigned char* load_image(const std::string&);
public:
    unsigned int id;
    GLenum internal_format, format, type;
    int attachment;

    Texture(GLenum = GL_RGB8, GLenum = GL_RGB, GLenum = GL_UNSIGNED_BYTE);
    Texture(const std::string&, GLenum = GL_RGB8, GLenum = GL_RGB, GLenum = GL_UNSIGNED_BYTE);
    Texture(int, int, GLenum = GL_RGB8, GLenum = GL_RGB, GLenum = GL_UNSIGNED_BYTE);
    ~Texture() override;
    void delete_texture();
    std::string& get_path();
    int get_width();
    int get_height();
    int get_nr_channels();
    std::string* get_path_ptr();
    int* get_width_ptr();
    int* get_height_ptr();
    int* get_nr_channels_ptr();


    void active_bind(unsigned int);
    void resize(int, int);
    void bind() override;
    void unbind() override;
};
