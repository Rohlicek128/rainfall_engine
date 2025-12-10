#pragma once
#include "engine/world/Component.h"
#include <string>

class Texture;
class Cubemap;
class TextureManager;

class TextureComponent : public Component
{
    int type_edit_;

    Texture* diffuse_texture_;
    Texture* roughness_texture_;
    Texture* metallic_texture_;
    Texture* normal_texture_;

    Cubemap* cubemap_;

    TextureManager* texture_manager_;

    void texture_gui(Texture*&, const std::string&);
public:
    float scale;
    int type;

    TextureComponent(int type, int diffuse = -1, int roughness = -1, int metal = -1, int normal = -1, int cubemap = -1);
    TextureComponent(Texture* diffuse = nullptr, Texture* roughness = nullptr, Texture* metal = nullptr, Texture* normal = nullptr, int type = 0xde1);
    ~TextureComponent() override = default;
    void active_bind(unsigned int);
    bool has_normal();
    bool has_roughness();
    bool has_metallic();

    std::string get_name() override;
    void set_gui() override;
    void serialize(YAML::Emitter& out) override;
    bool deserialize(YAML::Node& node) override;
};
