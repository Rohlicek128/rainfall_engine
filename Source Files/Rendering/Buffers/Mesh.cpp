#include "Mesh.h"

#include <iostream>

Mesh::Mesh(VertexAttribute* attribs, const int length)
{
    vbo_ = nullptr;
    ebo_ = nullptr;
    vao_ = nullptr;
    attributes = attribs;
    attrib_lenght = length;
    //entities = {};
}

void Mesh::bind()
{
    vao_->bind();
    vbo_->bind();
    ebo_->bind();
}

void Mesh::unbind()
{
    ebo_->unbind();
    vbo_->unbind();
    vao_->unbind();
}

void Mesh::compile()
{
    float* verts = concat_vertices();
    vbo_ = new VertexBuffer(verts, get_vertices_length());

    unsigned int* inds = concat_indices();
    ebo_ = new IndexBuffer(inds, get_indices_length());

    vao_ = new VertexArray(vbo_, attributes, attrib_lenght);
}

int Mesh::get_stride()
{
    int stride = 0;
    for (int i = 0; i < attrib_lenght; ++i) stride += attributes[i].lenght;
    return stride;
}

void Mesh::add_entity(Entity& entity)
{
    entities.push_back(std::make_shared<Entity>(entity));
}

void Mesh::add_entity(std::shared_ptr<Entity> entity)
{
    entities.push_back(std::move(entity));
}

float* Mesh::concat_vertices()
{
    int size = 0;
    for (const std::shared_ptr<Entity>& entity : entities)
    {
        if (entity->mesh == nullptr) continue;
        size += entity->mesh->verts_lenght;
    }
        
    
    float* verts = new float[size];
    int offset = 0;
    for (const std::shared_ptr<Entity>& entity : entities)
    {
        if (entity->mesh == nullptr) continue;
        
        for (int j = 0; j < entity->mesh->verts_lenght; ++j)
        {
            verts[offset + j] = entity->mesh->vertices[j];
        }
        offset += entity->mesh->verts_lenght;
    }
    
    return verts;
}

unsigned int* Mesh::concat_indices()
{
    int size = 0;
    for (const std::shared_ptr<Entity>& entity : entities)
    {
        if (entity->mesh == nullptr) continue;
        size += entity->mesh->inds_lenght;
    }
    
    unsigned int* inds = new unsigned int[size];
    int offset = 0;
    unsigned int vertex_offset = 0;
    for (const std::shared_ptr<Entity>& entity : entities)
    {
        if (entity->mesh == nullptr) continue;
        
        for (int j = 0; j < entity->mesh->inds_lenght; ++j)
        {
            inds[offset + j] = entity->mesh->indices[j] + vertex_offset;
        }
        offset += entity->mesh->inds_lenght;
        vertex_offset += entity->mesh->verts_lenght / get_stride();
    }

    return inds;
}

unsigned int Mesh::get_vertices_length()
{
    int length = 0;
    for (const std::shared_ptr<Entity>& entity : entities)
    {
        if (entity->mesh == nullptr) continue;
        length += entity->mesh->verts_lenght;
    }

    return length;
}

unsigned int Mesh::get_indices_length()
{
    if (ebo_ != nullptr) return ebo_->data_length;
    
    int length = 0;
    for (const std::shared_ptr<Entity>& entity : entities)
    {
        if (entity->mesh == nullptr) continue;
        length += entity->mesh->inds_lenght;
    }

    return length;
}
