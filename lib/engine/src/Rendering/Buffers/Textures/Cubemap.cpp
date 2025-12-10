#include "engine/rendering/Cubemap.h"

#include <iostream>
#include "stb_image.h"

Cubemap::Cubemap(const std::vector<std::string>& faces, const GLenum internal_format, const GLenum format, const GLenum type)
{
    this->internal_format = internal_format;
    this->format = format;
    this->type = type;

    glGenTextures(1, &handle_);
    Cubemap::bind();

    stbi_set_flip_vertically_on_load(false);
    for (unsigned int i = 0; i < 6; ++i)
        set_face(i, faces.at(i));

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    Cubemap::unbind();
}

void Cubemap::set_face(const unsigned int index, const std::string& path)
{
    faces[index] = std::make_unique<Texture>(internal_format, format, type);

    unsigned char* data = stbi_load(path.c_str(),
        faces[index]->get_width_ptr(), faces[index]->get_height_ptr(), faces[index]->get_nr_channels_ptr(), 0);
    *faces[index]->get_path_ptr() = path;

    if (data)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, faces[index]->internal_format,
            faces[index]->get_width(), faces[index]->get_height(), 0,
            faces[index]->format, faces[index]->type, data);
    }
    else
    {
        std::cout << "Loading Cube maps texture: \"" + path + "\": FAILED\n";
    }
    stbi_image_free(data);
}

void Cubemap::bind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle_);
}

void Cubemap::unbind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
