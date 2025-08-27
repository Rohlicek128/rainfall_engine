#include "Texture.h"

#include <iostream>
#include <stb/stb_image.h>

Texture::Texture(const std::string& path, const GLenum internal_format, const GLenum format, const GLenum type)
{
    path_ = path;
    internal_format_ = internal_format;
    format_ = format;
    type_ = type;
    
    glGenTextures(1, &handle_);
    Texture::bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    void* data = load_image(path);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format_, width_, height_, 0, format_, type_, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Loading texture \"" + path + "\": FAILED\n";
        glDeleteTextures(1, &handle_);
        handle_ = 3;
    }
    Texture::unbind();
    
    stbi_image_free(data);
}

Texture::Texture(const int width, const int height, const GLenum internal_format, const GLenum format, const GLenum type)
{
    path_ = "N/A";
    width_ = width;
    height_ = height;
    nr_channels_ = -1;
    internal_format_ = internal_format;
    format_ = format;
    type_ = type;
    
    glGenTextures(1, &handle_);
    Texture::bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width_, height_, 0, format, type, nullptr);

    Texture::unbind();
}

Texture::~Texture()
{
    //glDeleteTextures(1, &handle_);
}

std::string& Texture::get_path() { return path_; }

int Texture::get_width() { return width_; }

int Texture::get_height() { return height_; }

int Texture::get_nr_channels() { return nr_channels_; }

unsigned char* Texture::load_image(const std::string& path)
{
    stbi_set_flip_vertically_on_load(true);
    return stbi_load(("assets/" + path).c_str(), &width_, &height_, &nr_channels_, 0);
}

void Texture::active_bind(const unsigned int index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    bind();
}

void Texture::resize(const int width, const int height)
{
    width_ = width;
    height_ = height;
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format_, width_, height_, 0, format_, type_, nullptr);
    unbind();
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, handle_);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
