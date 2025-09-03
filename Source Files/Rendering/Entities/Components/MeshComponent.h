#pragma once

#include "Component.h"

class Mesh;

class MeshComponent : public Component
{
    Mesh* mesh_;
    bool is_mesh_view_open_;
public:
    int model_index;

    int primitive_type;
    bool is_inverted;
    bool is_culled;
    bool is_shaded;

    MeshComponent(int, int = 4, Mesh* = nullptr);
    ~MeshComponent() override = default;
    void set_mesh_manager(Mesh*&);
    void set_gui() override;
};