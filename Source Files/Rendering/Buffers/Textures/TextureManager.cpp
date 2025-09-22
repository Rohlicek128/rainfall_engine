#include "TextureManager.h"

#include "../../../Utils/FileDialogs.h"

TextureManager* TextureManager::instance_ptr_ = nullptr;

TextureManager::TextureManager()
{
    select_scale_ = 100.0f;
    load_as_srgb_ = false;
}

TextureManager* TextureManager::get_instance()
{
    if (instance_ptr_ == nullptr)
    {
        instance_ptr_ = new TextureManager();
    }
    return instance_ptr_;
}

void TextureManager::add_essential_texture(std::unique_ptr<Texture> texture)
{
    essential_textures_.push_back(std::move(texture));
}

Texture* TextureManager::get_essential_texture(const int index)
{
    return essential_textures_.at(index).get();
}

void TextureManager::add_texture(std::unique_ptr<Texture> texture)
{
    textures_.push_back(std::move(texture));
}

bool TextureManager::add_texture_open_file(const bool is_srgb)
{
    const std::string path = FileDialogs::open_file("Image File (*.jpg;*.png)\0*.jpg;*.png\0");
    if (path.empty()) return false;

    const bool is_png = !path.compare(path.length() - 3, 3, "png");
    int i_format = is_png ? (is_srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8) : GL_RGB8;
    int format = is_png ? GL_RGBA : GL_RGB;
    add_texture(std::make_unique<Texture>(path, i_format, format));
    
    return true;
}

Texture* TextureManager::get_texture(const int index)
{
    return textures_.at(index).get();
}

Texture* TextureManager::get_texture_by_handle(const unsigned int handle)
{
    for (int i = 0; i < essential_textures_.size(); ++i)
    {
        if (essential_textures_.at(i)->get_handle() == handle) return essential_textures_.at(i).get();
    }
    
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
    for (int i = 0; i < textures_.size(); ++i)
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

    
    if (ImGui::Button("  Add..  "))
        add_texture_open_file(load_as_srgb_);

    ImGui::SameLine();
    ImGui::Checkbox("Load SRGB", &load_as_srgb_);

    return 0;
}


void TextureManager::serialize(YAML::Emitter& out)
{
    out << YAML::Key << "Textures" << YAML::Value << YAML::BeginSeq;
    for (unsigned int i = 0; i < textures_.size(); ++i)
    {
        out << YAML::Flow << YAML::BeginSeq;
        out << textures_.at(i)->get_path() << textures_.at(i)->internal_format << textures_.at(i)->format;
        out << YAML::EndSeq;
    }
    out << YAML::EndSeq;
}

bool TextureManager::deserialize(YAML::Node& node)
{
    if (YAML::Node textures_des = node["Textures"])
    {
        for (int i = 0; i < textures_.size(); ++i)
        {
            textures_.at(i)->delete_texture();
        }
        textures_.clear();
        
        for (auto texture_des : textures_des)
        {
            YAML::iterator cur_node = texture_des.begin();
            std::string path = cur_node->as<std::string>();
            int in_for = (++cur_node)->as<int>();
            int format = (++cur_node)->as<int>();
            add_texture(std::make_unique<Texture>(path, in_for, format));
        }
    }
    
    return true;
}