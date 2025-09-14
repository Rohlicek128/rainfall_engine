#pragma once
#include "Component.h"
#include <string>

class TextureManager;

class TextureComponent : public Component
{
    int type_edit_;
    
    unsigned int diffuse_handle_;
    unsigned int roughness_handle_;
    unsigned int metallic_handle_;
    unsigned int normal_handle_;
    
    unsigned int cubemap_handle_;

    TextureManager* texture_manager_;

    void texture_gui(unsigned int*, const std::string&);
public:
    float scale;
    int type;
    
    TextureComponent(int, unsigned int = 1, unsigned int = 1, unsigned int = 1, int = -1);
    ~TextureComponent() override = default;
    void active_bind(unsigned int);
    int has_normal();
    int has_roughness();
    int has_metallic();

    std::string get_name() override;
    void set_gui() override;
    void serialize(YAML::Emitter& out) override;
    bool deserialize(YAML::Node& node) override;
};
