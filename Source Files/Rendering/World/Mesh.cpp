#include "Mesh.h"

#include <iostream>

Mesh::Mesh(const std::vector<VertexAttribute>& attribs)
{
    vbo_ = nullptr;
    ebo_ = nullptr;
    vao_ = nullptr;
    attributes = attribs;

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
    int stride = 0;
    for (int i = 0; i < attributes.size(); ++i) stride += attributes.at(i).lenght;
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
    
    models_.push_back(std::move(model));
}

ModelData* Mesh::get_model(const int index)
{
    if (index < 0 || index >= models_.size()) return nullptr;
    return models_.at(index).get();
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
    for (const std::unique_ptr<ModelData>& model : models_)
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