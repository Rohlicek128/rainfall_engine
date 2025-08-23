#pragma once
#include <vector>

#include "Texture.h"
#include "../../Gui/IGui.h"

class TextureManager : IGui
{
    std::vector<Texture*> textures_;
    float select_scale_;
    
    static TextureManager* instance_ptr_;

    TextureManager();
public:
    static TextureManager* get_instance();
    
    TextureManager(const TextureManager&) = default;
    ~TextureManager() override = default;
    
    void add_texture(Texture*);
    Texture* get_texture(int);
    Texture* get_texture_by_handle(unsigned int);
    
    void set_gui() override;
    unsigned int select_texture_gui();
};
