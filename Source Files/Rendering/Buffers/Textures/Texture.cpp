#include "Texture.h"

#include <iostream>
#include <stb/stb_image.h>

Texture::Texture(const std::string& path, const GLenum format)
{
    path_ = path;
    
    glGenTextures(1, &handle_);
    Texture::bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    void* data = load_image(path);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width_, height_, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Loading texture \"" + path + "\": FAILED\n";
        handle_ = 3;
    }
    
    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &handle_);
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

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, handle_);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
