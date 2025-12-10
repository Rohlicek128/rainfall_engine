#include "engine/rendering/Texture.h"

#include <filesystem>
#include <iostream>
#include "stb_image.h"

#include "TextureManager.h"
#include "../../../Utils/StringHelper.h"

Texture::Texture(const GLenum internal_format, const GLenum format, const GLenum type)
{
    path_ = "N/A";
    width_ = -1;
    height_ = -1;
    nr_channels_ = -1;
    this->internal_format = internal_format;
    this->format = format;
    this->type = type;
    attachment = -1;
    id = TextureManager::get_instance()->get_new_id();
}

Texture::Texture(const std::string& path, const GLenum internal_format, const GLenum format, const GLenum type)
{
    path_ = path;
    this->internal_format = internal_format;
    this->format = format;
    this->type = type;
    attachment = -1;
    id = TextureManager::get_instance()->get_new_id();

    glGenTextures(1, &handle_);
    Texture::bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    float anisotropy = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &anisotropy);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, anisotropy);

    void* data = load_image(path);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width_, height_, 0, format, type, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Loading texture \"" << path << "\": FAILED\n";
        glDeleteTextures(1, &handle_);
        handle_ = TextureManager::get_instance()->get_essential_texture(2)->get_handle();
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
    this->internal_format = internal_format;
    this->format = format;
    this->type = type;
    attachment = -1;
    id = TextureManager::get_instance()->get_new_id();

    glGenTextures(1, &handle_);
    Texture::bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width_, height_, 0, format, type, nullptr);

    Texture::unbind();
}

Texture::~Texture()
{
    //glDeleteTextures(1, &handle_);
}

void Texture::delete_texture()
{
    glDeleteTextures(0, &handle_);
}

std::string& Texture::get_path() { return path_; }
int Texture::get_width() { return width_; }
int Texture::get_height() { return height_; }
int Texture::get_nr_channels() { return nr_channels_; }

std::string* Texture::get_path_ptr() {return &path_; }
int* Texture::get_width_ptr() { return &width_; }
int* Texture::get_height_ptr() { return &height_; }
int* Texture::get_nr_channels_ptr() { return &nr_channels_; }


unsigned char* Texture::load_image(const std::string& path)
{
    stbi_set_flip_vertically_on_load(true);
    const std::string str = StringHelper::unescape_string(path);
    //std::cout << std::filesystem::current_path().string() << "\n" << path << "\n";
    return stbi_load(path.c_str(), &width_, &height_, &nr_channels_, 0);
}

void Texture::active_bind(const unsigned int index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    bind();
}

void Texture::resize(const int width, const int height)
{
    if (width_ == width && height_ == height) return;
    width_ = width;
    height_ = height;
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width_, height_, 0, format, type, nullptr);
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
