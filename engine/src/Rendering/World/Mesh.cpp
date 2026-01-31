#include "engine/managers/Mesh.h"

#include "../Buffers/IndexBuffer.h"
#include "../Buffers/VertexArray.h"
#include "engine/world/Entity.h"

#include <iostream>

Mesh::Mesh(const std::vector<VertexAttribute>& attribs, const bool tangent)
{
    vbo_ = nullptr;
    ebo_ = nullptr;
    vao_ = nullptr;
    attributes = attribs;

    add_tangent_ = tangent;
    if (add_tangent_) attributes.emplace_back("Tangent", 3);

    stride_ = 0;
    stride_ = get_stride();

    selected_mesh_ = 0;
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

    vao_ = new VertexArray(vbo_, attributes);
}

int Mesh::get_model_indices_offset(const int index)
{
    int offset = 0;
    for (int i = 0; i < models_.size(); ++i)
    {
        if (i == index) return offset;
        offset += models_.at(i)->indices_length;
    }
    return -1;
}

int Mesh::get_stride()
{
    if (stride_ != 0 || attributes.empty()) return stride_;
    int stride = 0;
    for (int i = 0; i < attributes.size(); ++i) stride += attributes.at(i).lenght;
    return stride;
}

void Mesh::add_model(const std::string& name, float verts[], const int v_length, unsigned int inds[], const int i_length)
{
    std::unique_ptr<ModelData> model = std::make_unique<ModelData>();
    model->name = name;

    if (!add_tangent_) model->vertices_length = v_length;
    else model->vertices_length = v_length + v_length / (get_stride() - 3) * 3;
    model->vertices = new float[v_length];
    std::copy_n(verts, v_length, model->vertices);

    model->indices_length = i_length;
    model->indices = new unsigned int[i_length];
    std::copy_n(inds, i_length, model->indices);

    models_.push_back(std::move(model));
}

ModelData* Mesh::get_model(const int index)
{
    if (index < 0 || index >= models_.size()) return nullptr;
    return models_.at(index).get();
}

glm::vec3 Mesh::calculate_tangent(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec2& uv1, const glm::vec2& uv2, const glm::vec2& uv3)
{
    const glm::vec3 e1 = v2 - v1;
    const glm::vec3 e2 = v3 - v1;
    const glm::vec2 d1 = uv2 - uv1;
    const glm::vec2 d2 = uv3 - uv1;

    const float f = 1.0f / (d1.x * d2.y - d2.x * d1.y);

    return {
        f * (d2.y * e1.x - d1.y * e2.x),
        f * (d2.y * e1.y - d1.y * e2.y),
        f * (d2.y * e1.z - d1.y * e2.z)
    };
}

float* Mesh::concat_vertices()
{
    int size = 0;
    for (const std::unique_ptr<ModelData>& model : models_)
    {
        if (model == nullptr) continue;
        size += model->vertices_length;
    }

    float* verts = new float[size];
    int offset = 0;
    const int tangent_stride = add_tangent_ ? attributes.back().lenght : 0;
    for (const std::unique_ptr<ModelData>& model : models_)
    {
        if (model == nullptr) continue;

        glm::vec3 t_pos[3] {glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)};
        glm::vec2 t_uv[3] {glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f)};
        for (int i = 0; i < model->indices_length / 3; ++i) // Each triangle
        {
            for (int j = 0; j < 3; ++j) // Each vertex
            {
                t_pos[j].x = model->vertices[model->indices[i * 3 + j] * (stride_ - tangent_stride) + 0];
                t_pos[j].y = model->vertices[model->indices[i * 3 + j] * (stride_ - tangent_stride) + 1];
                t_pos[j].z = model->vertices[model->indices[i * 3 + j] * (stride_ - tangent_stride) + 2];
                verts[offset + model->indices[i * 3 + j] * stride_ + 0] = t_pos[j].x;
                verts[offset + model->indices[i * 3 + j] * stride_ + 1] = t_pos[j].y;
                verts[offset + model->indices[i * 3 + j] * stride_ + 2] = t_pos[j].z;

                t_uv[j].x = model->vertices[model->indices[i * 3 + j] * (stride_ - tangent_stride) + 3];
                t_uv[j].y = model->vertices[model->indices[i * 3 + j] * (stride_ - tangent_stride) + 4];
                verts[offset + model->indices[i * 3 + j] * stride_ + 3] = t_uv[j].x;
                verts[offset + model->indices[i * 3 + j] * stride_ + 4] = t_uv[j].y;
            }

            glm::vec3 tangent;
            if (add_tangent_) tangent = calculate_tangent(t_pos[0], t_pos[1], t_pos[2], t_uv[0], t_uv[1], t_uv[2]);

            for (int j = 0; j < 3; ++j)
            {
                for (int k = 5; k < stride_ - tangent_stride; ++k)
                    verts[offset + model->indices[i * 3 + j] * stride_ + k] = model->vertices[model->indices[i * 3 + j] * (stride_ - tangent_stride) + k];
                if (add_tangent_)
                {
                    verts[offset + model->indices[i * 3 + j] * stride_ + (stride_ - 3)] = tangent.x;
                    verts[offset + model->indices[i * 3 + j] * stride_ + (stride_ - 2)] = tangent.y;
                    verts[offset + model->indices[i * 3 + j] * stride_ + (stride_ - 1)] = tangent.z;
                }
            }
            /*if (add_tangent_)
            {
                for (int j = 0; j < 3; ++j)
                {
                    for (int k = 0; k < stride_; ++k)
                        std::cout << verts[offset + model->indices[i * 3 + j] * stride_ + k] << ' ';
                    std::cout << '\n';
                }
            }*/
        }
        offset += model->vertices_length;
    }

    return verts;
}

unsigned int* Mesh::concat_indices()
{
    int size = 0;
    for (const std::unique_ptr<ModelData>& model : models_)
    {
        if (model == nullptr) continue;
        size += model->indices_length;
    }

    unsigned int* inds = new unsigned int[size];
    int offset = 0;
    unsigned int vertex_offset = 0;
    for (const std::unique_ptr<ModelData>& model : models_)
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
    for (const std::unique_ptr<ModelData>& model : models_)
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
    for (const std::unique_ptr<ModelData>& model : models_)
    {
        if (model == nullptr) continue;
        length += model->indices_length;
    }
    return length;
}


void Mesh::set_default_models()
{
    float vertices_cube[] = {
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // A 0
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // B 1
        1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // C 2
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // D 3

        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // E 4
        1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // F 5
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // G 6
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // H 7

        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // D 8
        -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // A 9
        -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // E 10
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // H 11

        1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // B 12
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // C 13
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // G 14
        1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // F 15

        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // A 16
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // B 17
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, // F 18
        -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // E 19

        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // C 20
        -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // D 21
        -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // H 22
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f // G 23
    };
    unsigned int indices_cube[] = {
        // front and back
        0, 3, 2,
        2, 1, 0,
        4, 5, 6,
        6, 7, 4,
        // left and right
        11, 8, 9,
        9, 10, 11,
        12, 13, 14,
        14, 15, 12,
        // bottom and top
        16, 17, 18,
        18, 19, 16,
        20, 21, 22,
        22, 23, 20
    };
    add_model("Cube", vertices_cube, std::size(vertices_cube), indices_cube, std::size(indices_cube));

    float vertices_plane[] = {
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };
    unsigned int indices_plane[] = {
        0, 1, 2,
        2, 3, 0
    };
    add_model("Plane", vertices_plane, std::size(vertices_plane), indices_plane, std::size(indices_plane));

    float vertices_paper[] = {
        0.0f, 0.0f, 1.0f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, // 0
        -1.0f, -0.1f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 1
        1.0f, -0.1f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 2
        0.0f, 0.2f, -1.0f, 0.5f, 0.2f, 0.0f, 1.0f, 0.0f, // 3
        -0.2f, 0.0f, -1.0f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f, // 4
        0.2f, 0.0f, -1.0f, 0.7f, 0.0f, 0.0f, 1.0f, 0.0f // 5
    };
    unsigned int indices_paper[] = {
        0, 4, 5,
        0, 3, 4,
        0, 5, 3,
        1, 4, 3,
        1, 3, 0,
        2, 3, 5,
        2, 0, 3
    };
    add_model("Paper plane", vertices_paper, std::size(vertices_paper), indices_paper, std::size(indices_paper));
}
