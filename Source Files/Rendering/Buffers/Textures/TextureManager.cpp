#include "TextureManager.h"

TextureManager* TextureManager::instance_ptr_ = nullptr;

TextureManager::TextureManager()
{
    select_scale_ = 100.0f;
}

TextureManager* TextureManager::get_instance()
{
    if (instance_ptr_ == nullptr)
    {
        instance_ptr_ = new TextureManager();
    }
    return instance_ptr_;
}

void TextureManager::add_texture(std::unique_ptr<Texture> texture)
{
    textures_.push_back(std::move(texture));
}

Texture* TextureManager::get_texture(const int index)
{
    return textures_.at(index).get();
}

Texture* TextureManager::get_texture_by_handle(const unsigned int handle)
{
    for (int i = 0; i < textures_.size(); ++i)
    {
        if (textures_.at(i)->get_handle() == handle) return textures_.at(i).get();
    }
    return nullptr;
}

void TextureManager::add_cubemap(std::unique_ptr<Cubemap> cubemap)
{
    cubemaps_.push_back(std::move(cubemap));
}

Cubemap* TextureManager::get_cubemap(const int index)
{
    return cubemaps_.at(index).get();
}


void TextureManager::set_gui()
{
    select_texture_2d_gui();
}

unsigned int TextureManager::select_texture_2d_gui()
{
    ImGui::SeparatorText("Select a texture:");
    
    ImGui::SliderFloat("Scale", &select_scale_, 30.0f, 400.0f, "%.0f");
    float max_width = 0.0;

    float width_aspect = 0.0f;
    for (int i = 3; i < textures_.size(); ++i)
    {
        width_aspect = (float)textures_.at(i)->get_width() / (float)textures_.at(i)->get_height();
        max_width += select_scale_ * width_aspect;
        //Image
        ImGui::PushID(i);
        if (ImGui::ImageButton("", (ImTextureID)(intptr_t)textures_.at(i)->get_handle(),
            ImVec2(select_scale_ * width_aspect, select_scale_), {0, 1}, {1, 0}))
        {
            ImGui::PopID();
            return textures_.at(i)->get_handle();
        }
        ImGui::PopID();

        //Tooltip
        if (ImGui::BeginItemTooltip())
        {
            ImGui::Text(textures_.at(i)->get_path().c_str());
            if (textures_.at(i)->get_handle() != 3)
            {
                ImGui::Text("Handle: %i", textures_.at(i)->get_handle());
                ImGui::Text("Width: %i", textures_.at(i)->get_width());
                ImGui::Text("Height: %i", textures_.at(i)->get_height());
                ImGui::Text("Channels: %i", textures_.at(i)->get_nr_channels());
            }
            else ImGui::Text("FAILED TO LOAD");
            
            
            ImGui::EndTooltip();
        }
        
        if (max_width <= 800.0 - select_scale_) ImGui::SameLine();
        else max_width = 0.0;
    }
    ImGui::NewLine();
    if (ImGui::Button("  None  ")) return 1;

    return 0;
}
