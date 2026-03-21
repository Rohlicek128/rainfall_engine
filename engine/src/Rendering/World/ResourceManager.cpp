#include "engine/managers/ResourceManager.h"

#include "engine/managers/TextureManager.h"
#include <memory>

//#include "assimp/Importer.hpp"
//#include "assimp/scene.h"
//#include "assimp/postprocess.h"

#include <glad.h>
#include <vector>

namespace engine
{
    ResourceManager::ResourceManager()
    {
        mesh_manager_ = std::make_unique<Mesh>(std::vector<VertexAttribute>{{"Position", 3}, {"TexCoord", 2}, {"Normal", 3}}, true);
        mesh_manager_->set_default_models();

        texture_manager_ = TextureManager::get_instance();
    }

    void ResourceManager::reset()
    {
        texture_manager_->reset();
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


    int ResourceManager::load_model(const std::string& path)
    {
        /*Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            return -1;
        }
        processNode(scene->mRootNode, scene);*/

        return 0;
    }

    /*void processNode(aiNode *node, const aiScene *scene)
    {
        //std::vector<>

        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            //meshes.push_back(processMesh(mesh, scene));
        }

        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }*/
}
