#include "engine/managers/ResourceManager.h"

#include "engine/managers/TextureManager.h"
#include <memory>
#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

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
        mesh_manager_->reset();
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
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return -1;
        }

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        unsigned int index_offset = 0;

        for (unsigned int m = 0; m < scene->mNumMeshes; m++)
        {
            process_loaded_mesh(vertices, indices, scene->mMeshes[m], &index_offset);
        }

        std::string name = path.substr(path.find_last_of("/\\") + 1);
        name = name.substr(0, name.find_last_of('.'));

        return mesh_manager_->add_model(name, vertices.data(), vertices.size(), indices.data(), indices.size(), path);
    }

    void ResourceManager::process_loaded_mesh(std::vector<float>& vertices, std::vector<unsigned int>& indices, aiMesh* mesh, unsigned int* offset)
    {
        // Vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            // Position
            vertices.push_back(mesh->mVertices[i].x);
            vertices.push_back(mesh->mVertices[i].y);
            vertices.push_back(mesh->mVertices[i].z);

            // TexCoords
            if (mesh->mTextureCoords[0])
            {
                vertices.push_back(mesh->mTextureCoords[0][i].x);
                vertices.push_back(mesh->mTextureCoords[0][i].y);
            }
            else
            {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            // Normals
            if (mesh->HasNormals())
            {
                vertices.push_back(mesh->mNormals[i].x);
                vertices.push_back(mesh->mNormals[i].y);
                vertices.push_back(mesh->mNormals[i].z);
            }
            else
            {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
        }

        // Indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j] + *offset);
            }
        }

        *offset += mesh->mNumVertices;
    }
}
