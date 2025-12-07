#include "engine/managers/ResourceManager.h"

#include "../Buffers/Textures/TextureManager.h"
#include <memory>

#include <glad.h>

namespace engine
{
    ResourceManager::ResourceManager()
    {
        mesh_manager_ = std::make_unique<Mesh>(std::vector<VertexAttribute>{{"Position", 3}, {"TexCoord", 2}, {"Normal", 3}}, true);
        mesh_manager_->set_default_models();

        texture_manager_ = TextureManager::get_instance();
    }

    Mesh* ResourceManager::get_mesh_manager()
    {
        return mesh_manager_.get();
    }

    TextureManager* ResourceManager::get_texture_manager()
    {
        return texture_manager_;
    }


    Texture* ResourceManager::load_texture(const std::string& path, const std::string& format)
    {
        GLenum gl_internal_format;
        GLenum gl_format;
        if (format == "png")
        {
            gl_internal_format = GL_SRGB8_ALPHA8;
            gl_format = GL_RGBA;
        }
        else if (format == "jpg" || format == "jpeg")
        {
            gl_internal_format = GL_SRGB8;
            gl_format = GL_RGB;
        }
        else return nullptr;

        return texture_manager_->add_texture(std::make_unique<Texture>(path, gl_internal_format, gl_format));
    }

    Texture* ResourceManager::get_texture_by_id(unsigned int id)
    {
        return texture_manager_->get_texture_by_id(id);
    }
}
