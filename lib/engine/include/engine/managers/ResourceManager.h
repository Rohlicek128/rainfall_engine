#pragma once

#include <memory>
#include <string>

#include "Mesh.h"


class TextureManager;
class Texture;

namespace engine
{
    class ResourceManager
    {
        std::unique_ptr<Mesh> mesh_manager_;
        TextureManager* texture_manager_;

    public:
        ResourceManager();

        Mesh* get_mesh_manager();
        TextureManager* get_texture_manager();

        Texture* load_texture(const std::string& path, const std::string& format);
        Texture* get_texture_by_id(unsigned int id);

    };
}
