#pragma once
#include "Component.h"
#include "glad/glad.h"

class MeshComponent : public Component
{
    int type_edit_;
    
public:
    float* vertices;
    int verts_lenght;
    unsigned int* indices;
    int inds_lenght;

    GLenum primitive_type;

    MeshComponent(float[], unsigned int[], int, int, GLenum = GL_TRIANGLES);
    ~MeshComponent() override = default;
    void set_gui() override;
};
