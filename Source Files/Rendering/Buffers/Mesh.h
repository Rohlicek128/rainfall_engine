#pragma once
#include <memory>
#include <vector>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "../Entities/Entity.h"

class Mesh
{
    VertexArray* vao_;
    VertexBuffer* vbo_;
    IndexBuffer* ebo_;

    float* concat_vertices();
    unsigned int* concat_indices();
    unsigned int get_vertices_length();
    unsigned int get_indices_length();
public:
    std::vector<std::shared_ptr<Entity>> entities;
    VertexAttribute* attributes;
    int attrib_lenght;
    
    Mesh(VertexAttribute*, int);
    void bind();
    void unbind();
    void compile();

    int get_stride();
    void add_entity(Entity&);
    void add_entity(std::shared_ptr<Entity>);
    Entity* find_entity_by_id(unsigned int);
};