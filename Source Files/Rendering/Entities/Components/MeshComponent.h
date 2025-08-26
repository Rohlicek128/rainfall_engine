#pragma once
#include "glad/glad.h"

#include "Component.h"

class MeshComponent : public Component
{
    int type_edit_;
    
public:
    int model_index;

    GLenum primitive_type;

    MeshComponent(int, GLenum = GL_TRIANGLES);
    ~MeshComponent() override = default;
    void set_gui() override;
};
