#pragma once
#include <memory>
#include <vector>

#include "Cubemap.h"
#include "Texture.h"
#include "../../Gui/IGui.h"
#include "engine/core/ISerializable.h"

class TextureManager : IGui, public ISerializable
{
    std::vector<std::unique_ptr<Texture>> essential_textures_;
    std::vector<std::unique_ptr<Texture>> textures_;
    std::vector<std::unique_ptr<Cubemap>> cubemaps_;
    float select_scale_;
    bool load_as_srgb_;
    unsigned int id_counter_;

    static TextureManager* instance_ptr_;

    TextureManager();
public:
    static TextureManager* get_instance();

    TextureManager(const TextureManager&) = default;
    ~TextureManager() override = default;

    unsigned int get_new_id();

    void add_essential_texture(std::unique_ptr<Texture>);
    Texture* get_essential_texture(int);

    Texture* add_texture(std::unique_ptr<Texture>);
    Texture* add_texture_open_file(bool is_srgb);
    Texture* get_texture(int);
    Texture* get_texture_by_handle(unsigned int);
    Texture* get_texture_by_id(unsigned int);

    void add_cubemap(std::unique_ptr<Cubemap>);
    Cubemap* get_cubemap(int);

    void serialize(YAML::Emitter& out) override;
    bool deserialize(YAML::Node& node) override;
    void set_gui() override;
    Texture* select_texture_2d_gui();
};
