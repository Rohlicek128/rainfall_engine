#pragma once
#include "engine/core/ISerializable.h"

class TransformComponent;

enum COMPONENTS_IDS
{
    TRANSFORM,
    MESH,
    CAMERA,
    MATERIAL,
    TEXTURE,
    LIGHT
};

inline const char* to_string(const COMPONENTS_IDS e)
{
    switch (e)
    {
    case TRANSFORM: return "TRANSFORM";
    case MESH: return "MESH";
    case CAMERA: return "CAMERA";
    case MATERIAL: return "MATERIAL";
    case TEXTURE: return "TEXTURE";
    case LIGHT: return "LIGHT";
    default: return "Unknown";
    }
}

class Component : public ISerializable
{
public:
    bool is_enabled = true;
    virtual std::string get_name() = 0;
};
