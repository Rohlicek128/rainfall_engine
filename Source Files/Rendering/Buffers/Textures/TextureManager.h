#pragma once
#include <memory>
#include <vector>

#include "Cubemap.h"
#include "Texture.h"
#include "../../Gui/IGui.h"

class TextureManager : IGui
{
    std::vector<std::unique_ptr<Texture>> textures_;
    std::vector<std::unique_ptr<Cubemap>> cubemaps_;
    float select_scale_;
    
    static TextureManager* instance_ptr_;

    TextureManager();
public:
    static TextureManager* get_instance();
    
    TextureManager(const TextureManager&) = default;
    ~TextureManager() override = default;
    
    void add_texture(std::unique_ptr<Texture>);
    Texture* get_texture(int);
    Texture* get_texture_by_handle(unsigned int);

    void add_cubemap(std::unique_ptr<Cubemap>);
    Cubemap* get_cubemap(int);
    
    void set_gui() override;
    unsigned int select_texture_2d_gui();
};
