#pragma once

#include <memory>
#include <string>

#include "Mesh.h"
#include "../rendering/Texture.h"


class TextureManager;

class aiMesh;

namespace engine
{
    class ResourceManager
    {
        std::unique_ptr<Mesh> mesh_manager_;
        TextureManager* texture_manager_;
        void process_loaded_mesh(std::vector<float>& vertices, std::vector<unsigned int>& indices, aiMesh* mesh, unsigned int* offset);

    public:
        ResourceManager();

        void reset();

        Mesh* get_mesh_manager();
        TextureManager* get_texture_manager();

        Texture* load_texture(const std::string& path, const std::string& format);
        Texture* get_texture_by_id(unsigned int id);

        int load_model(const std::string& path);
    };
}
