#pragma once
#include "../../Gui/IGui.h"

enum components_ids
{
    TRANSFORM,
    MESH,
    CAMERA,
    MATERIAL,
    TEXTURE,
    LIGHT
};

inline const char* to_string(const components_ids e)
{
    switch (e)
    {
    case TRANSFORM: return "Transform";
    case MESH: return "Mesh";
    case CAMERA: return "Camera";
    case MATERIAL: return "Material";
    case TEXTURE: return "Texture";
    case LIGHT: return "Light";
    default: return "N/A";
    }
}

class Component : public IGui
{
public:
    bool is_enabled = true;
};
