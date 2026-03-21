#pragma once

#include <memory>
#include <string>

#include "Mesh.h"
#include "../rendering/Texture.h"


class TextureManager;

//class aiNode;
//class aiScene;

namespace engine
{
    class ResourceManager
    {
        std::unique_ptr<Mesh> mesh_manager_;
        TextureManager* texture_manager_;
        //void processNode(aiNode *node, const aiScene *scene);

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
