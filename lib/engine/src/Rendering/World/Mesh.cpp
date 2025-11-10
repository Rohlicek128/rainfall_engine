#include "Mesh.h"

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

int Mesh::select_mesh_gui()
{
    int result = -1;
    //Index
    if (ImGui::BeginChild("##MeshIndex", {20, 0}, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX))
    {
        for (int i = 0; i < models_.size(); ++i)
        {
            if (selected_mesh_ == i) ImGui::Text("%i", i);
            else ImGui::TextDisabled("%i", i);
        }
        ImGui::EndChild();
    }

    ImGui::SameLine();

    //Meshes
    if (ImGui::BeginChild("##Meshes", {150, 0}, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX))
    {
        for (int i = 0; i < models_.size(); ++i)
        {
            if (ImGui::Selectable(models_.at(i)->name.c_str(), selected_mesh_ == i, ImGuiSelectableFlags_AllowDoubleClick))
                selected_mesh_ = i;
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                result = selected_mesh_;
        }
        ImGui::EndChild();
    }

    ImGui::SameLine();

    //Description
    ImGui::BeginGroup();
    if (ImGui::BeginChild("##MeshesDesc", {0, -ImGui::GetFrameHeightWithSpacing()}))
    {
        ImGui::PushFont(nullptr, ImGui::GetStyle().FontSizeBase * 2.0f);
        ImGui::Text(get_model(selected_mesh_)->name.c_str());
        ImGui::PopFont();
        ImGui::TextDisabled("Size in memory: %.3f KB",
            (float)(get_model(selected_mesh_)->vertices_length * sizeof(float) + get_model(selected_mesh_)->indices_length * sizeof(unsigned int))
            / 8000.0f);
        ImGui::TextDisabled("Index: %i", selected_mesh_);

        ImGui::Separator();

        ImGui::Text("Vertices: %i", get_model(selected_mesh_)->vertices_length);
        if (ImGui::TreeNode("Vertices"))
        {
            ImGui::BeginChild("VerticesWritten", {0, 6 * ImGui::GetTextLineHeightWithSpacing()}, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY);
            for (int i = 0; i < models_.at(selected_mesh_)->vertices_length; ++i)
            {
                ImGui::Text("%i ", models_.at(selected_mesh_)->vertices[i]);
                const int debug = (i + 1) % get_stride();
                if (debug != 0) ImGui::SameLine();
            }
            ImGui::EndChild();
            ImGui::TreePop();
        }
        ImGui::Text("Indices: %i", get_model(selected_mesh_)->indices_length);
        if (ImGui::TreeNode("Indices"))
        {
            ImGui::BeginChild("IndicesWritten", {0, 6 * ImGui::GetTextLineHeightWithSpacing()}, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY);
            for (int i = 0; i < models_.at(selected_mesh_)->indices_length; ++i)
            {
                ImGui::Text("%i ", models_.at(selected_mesh_)->indices[i]);
                const int debug = (i + 1) % 3;
                if (debug != 0) ImGui::SameLine();
            }
            ImGui::EndChild();
            ImGui::TreePop();
        }
        
        ImGui::EndChild();
    }
    if (result == -1 && ImGui::Button("Select", {-FLT_MIN, ImGui::GetTextLineHeightWithSpacing()}))
        result = selected_mesh_;
    ImGui::EndGroup();

    return result;
}

void Mesh::set_gui()
{
    select_mesh_gui();
}