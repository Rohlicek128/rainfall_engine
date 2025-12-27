#include "TextureManager.h"

#include "../../../Utils/FileDialogs.h"

TextureManager* TextureManager::instance_ptr_ = nullptr;

TextureManager::TextureManager()
{
    select_scale_ = 100.0f;
    load_as_srgb_ = false;
    id_counter_ = 0;
}

TextureManager* TextureManager::get_instance()
{
    if (instance_ptr_ == nullptr)
    {
        instance_ptr_ = new TextureManager();
    }
    return instance_ptr_;
}

unsigned int TextureManager::get_new_id()
{
    return id_counter_++;
}

void TextureManager::add_essential_texture(std::unique_ptr<Texture> texture)
{
    essential_textures_.push_back(std::move(texture));
}

Texture* TextureManager::get_essential_texture(const int index)
{
    return essential_textures_.at(index).get();
}

Texture* TextureManager::add_texture(std::unique_ptr<Texture> texture)
{
    textures_.push_back(std::move(texture));
    return textures_.back().get();
}

Texture* TextureManager::add_texture_open_file(const bool is_srgb)
{
    const std::string path = FileDialogs::open_file("Image File (*.jpg;*.png)\0*.jpg;*.png\0");
    if (path.empty()) return nullptr;

    const bool is_png = !path.compare(path.length() - 3, 3, "png");
    int i_format = is_png ? (is_srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8) : GL_RGB8;
    int format = is_png ? GL_RGBA : GL_RGB;
    add_texture(std::make_unique<Texture>(path, i_format, format));

    return textures_.back().get();
}

Texture* TextureManager::get_texture(const int index)
{
    if (index < 0 || index >= textures_.size())
        return essential_textures_.at(2).get();
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

Texture* TextureManager::get_texture_by_id(const unsigned int id)
{
    for (int i = 0; i < essential_textures_.size(); ++i)
    {
        if (essential_textures_.at(i)->id == id) return essential_textures_.at(i).get();
    }

    for (int i = 0; i < textures_.size(); ++i)
    {
        if (textures_.at(i)->id == id) return textures_.at(i).get();
    }
    return essential_textures_.at(2).get();
}

void TextureManager::add_cubemap(std::unique_ptr<Cubemap> cubemap)
{
    cubemaps_.push_back(std::move(cubemap));
}

Cubemap* TextureManager::get_cubemap(const int index)
{
    return cubemaps_.at(index).get();
}


void TextureManager::serialize(YAML::Emitter& out)
{
    out << YAML::Key << "Texture Manager" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "ID Count" << YAML::Value << id_counter_;

    out << YAML::Key << "Textures" << YAML::Value << YAML::BeginSeq;
    for (unsigned int i = 0; i < textures_.size(); ++i)
    {
        out << YAML::Flow << YAML::BeginSeq;
        out << textures_.at(i)->id << textures_.at(i)->get_path() << textures_.at(i)->internal_format << textures_.at(i)->format;
        out << YAML::EndSeq;
    }
    out << YAML::EndSeq;
    out << YAML::EndMap;
}

bool TextureManager::deserialize(YAML::Node& node)
{
    if (!node["Texture Manager"]) return false;

    YAML::Node manager = node["Texture Manager"];

    for (int i = 0; i < textures_.size(); ++i)
    {
        textures_.at(i)->delete_texture();
    }
    textures_.clear();

    for (auto texture_des : manager["Textures"])
    {
        YAML::iterator cur_node = texture_des.begin();
        unsigned int id = cur_node->as<unsigned int>();
        std::string path = (++cur_node)->as<std::string>();
        bool skip = false;
        for (unsigned int i = 0; i < essential_textures_.size(); ++i)
        {
            if (path == essential_textures_.at(i)->get_path())
            {
                skip = true;
                break;
            }
        }
        if (skip) continue;

        int in_for = (++cur_node)->as<int>();
        int format = (++cur_node)->as<int>();
        add_texture(std::make_unique<Texture>(path, in_for, format));
        textures_.back()->id = id;
    }
    id_counter_ = manager["ID Count"].as<unsigned int>();

    return true;
}
