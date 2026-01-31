#pragma once
#include <string>

struct ModelData
{
    std::string name;
    float* vertices = nullptr;
    int vertices_length = 0;
    unsigned int* indices = nullptr;
    int indices_length = 0;

    ModelData() = default;

    ~ModelData() {
        delete[] vertices;
        delete[] indices;
    }
    
    ModelData(const ModelData&) = delete;
    ModelData& operator=(const ModelData&) = delete;
    
    ModelData(ModelData&& other) noexcept {
        *this = std::move(other);
    }
    ModelData& operator=(ModelData&& other) noexcept {
        if (this != &other) {
            delete[] vertices;
            delete[] indices;
            name = std::move(other.name);
            vertices = other.vertices;
            vertices_length = other.vertices_length;
            indices = other.indices;
            indices_length = other.indices_length;

            other.vertices = nullptr;
            other.indices = nullptr;
        }
        return *this;
    }
};
