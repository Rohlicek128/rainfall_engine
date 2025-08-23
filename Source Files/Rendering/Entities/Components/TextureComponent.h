#pragma once
#include "Component.h"
#include "../../Buffers/Textures/TextureManager.h"

class TextureComponent : public Component
{
    unsigned int diffuse_handle_;
    unsigned int specular_handle_;

    TextureManager* texture_manager_;

    void texture_gui(unsigned int*, const std::string&);
public:
    TextureComponent(unsigned int, unsigned int);
    ~TextureComponent() override = default;
    void active_bind(unsigned int);
    void set_gui() override;
};
