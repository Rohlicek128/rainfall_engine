#include "Mesh.h"

#include <iostream>

Mesh::Mesh(VertexAttribute* attribs, const int length)
{
    vbo_ = nullptr;
    ebo_ = nullptr;
    vao_ = nullptr;
    attributes = attribs;
    attrib_lenght = length;
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

int Mesh::get_model_indices_offset(const int index)
{
    int offset = 0;
    for (int i = 0; i < models.size(); ++i)
    {
        if (i == index) return offset;
        offset += models.at(i)->indices_length;
    }
    return -1;
}

int Mesh::get_stride()
{
    int stride = 0;
    for (int i = 0; i < attrib_lenght; ++i) stride += attributes[i].lenght;
    return stride;
}

void Mesh::add_model(const std::string& name, float verts[], const int v_length, unsigned int inds[], const int i_length)
{
    std::unique_ptr<ModelData> model = std::make_unique<ModelData>();
    model->name = name;

    model->vertices_length = v_length;
    model->vertices = new float[v_length];
    std::copy_n(verts, v_length, model->vertices);

    model->indices_length = i_length;
    model->indices = new unsigned int[i_length];
    std::copy_n(inds, i_length, model->indices);
    
    models.push_back(std::move(model));
}

float* Mesh::concat_vertices()
{
    int size = 0;
    for (const std::unique_ptr<ModelData>& model : models)
    {
        if (model == nullptr) continue;
        size += model->vertices_length;
    }
    
    float* verts = new float[size];
    int offset = 0;
    for (const std::unique_ptr<ModelData>& model : models)
    {
        if (model == nullptr) continue;
        
        for (int j = 0; j < model->vertices_length; ++j)
        {
            verts[offset + j] = model->vertices[j];
        }
        offset += model->vertices_length;
    }
    
    return verts;
}

unsigned int* Mesh::concat_indices()
{
    int size = 0;
    for (const std::unique_ptr<ModelData>& model : models)
    {
        if (model == nullptr) continue;
        size += model->indices_length;
    }
    
    unsigned int* inds = new unsigned int[size];
    int offset = 0;
    unsigned int vertex_offset = 0;
    for (const std::unique_ptr<ModelData>& model : models)
    {
        if (model == nullptr) continue;
        
        for (int j = 0; j < model->indices_length; ++j)
        {
            inds[offset + j] = model->indices[j] + vertex_offset;
        }
        offset += model->indices_length;
        vertex_offset += model->vertices_length / get_stride();
    }
    
    return inds;
}

unsigned int Mesh::get_vertices_length()
{
    int length = 0;
    for (const std::unique_ptr<ModelData>& model : models)
    {
        if (model == nullptr) continue;
        length += model->vertices_length;
    }
    return length;
}

unsigned int Mesh::get_indices_length()
{
    if (ebo_ != nullptr) return ebo_->data_length;
    
    int length = 0;
    for (const std::unique_ptr<ModelData>& model : models)
    {
        if (model == nullptr) continue;
        length += model->indices_length;
    }
    return length;
}