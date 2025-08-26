#pragma once
#include <memory>
#include <vector>

#include "ModelData.h"
#include "../Buffers/IndexBuffer.h"
#include "../Buffers/VertexArray.h"
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
    std::vector<std::unique_ptr<ModelData>> models;
    VertexAttribute* attributes;
    int attrib_lenght;
    
    Mesh(VertexAttribute*, int);
    void bind();
    void unbind();
    void compile();

    int get_model_indices_offset(int);
    int get_stride();
    void add_model(const std::string&, float[], int, unsigned int[], int);
};