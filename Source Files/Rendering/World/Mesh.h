#pragma once
#include <memory>
#include <vector>

#include "ModelData.h"
#include "../Buffers/IndexBuffer.h"
#include "../Buffers/VertexArray.h"
#include "../Entities/Entity.h"

class Mesh : public IGui
{
    VertexArray* vao_;
    VertexBuffer* vbo_;
    IndexBuffer* ebo_;

    std::vector<std::unique_ptr<ModelData>> models_;
    int selected_mesh_;

    float* concat_vertices();
    unsigned int* concat_indices();
    unsigned int get_vertices_length();
    unsigned int get_indices_length();
public:
    std::vector<VertexAttribute> attributes;
    
    Mesh(const std::vector<VertexAttribute>&);
    void bind();
    void unbind();
    void compile();

    int get_model_indices_offset(int);
    int get_stride();
    void add_model(const std::string&, float[], int, unsigned int[], int);
    ModelData* get_model(int);

    int select_mesh_gui();
    void set_gui() override;
};