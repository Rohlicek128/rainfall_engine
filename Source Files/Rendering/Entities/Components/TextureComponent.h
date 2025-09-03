#pragma once
#include "Component.h"
#include <string>

class TextureManager;

class TextureComponent : public Component
{
    int type_edit_;
    
    unsigned int diffuse_handle_;
    unsigned int specular_handle_;
    unsigned int normal_handle_;
    
    unsigned int cubemap_handle_;

    TextureManager* texture_manager_;

    void texture_gui(unsigned int*, const std::string&);
public:
    int type;
    
    TextureComponent(int, unsigned int = 1, unsigned int = 1, unsigned int = 1, int = -1);
    ~TextureComponent() override = default;
    void active_bind(unsigned int);
    int has_normal();
    void set_gui() override;
};
