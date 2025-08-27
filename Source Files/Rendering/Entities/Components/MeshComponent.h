#pragma once
#include "glad/glad.h"

#include "Component.h"

class Mesh;

class MeshComponent : public Component
{
    int type_edit_;

    Mesh* mesh_;
    bool is_mesh_view_open_;
public:
    int model_index;

    GLenum primitive_type;
    bool is_inverted;
    bool is_culled;
    bool is_shaded;

    MeshComponent(int, GLenum = GL_TRIANGLES, Mesh* = nullptr);
    ~MeshComponent() override = default;
    void set_mesh_manager(Mesh*&);
    void set_gui() override;
};