#include "engine/world/components/TextureComponent.h"

#include "../../Buffers/Textures/TextureManager.h"

TextureComponent::TextureComponent(const int type, const int diffuse, const int roughness, const int metal, const int normal, const int cubemap)
{
    this->type = type;
    if (type == GL_TEXTURE_2D) type_edit_ = 0;
    else type_edit_ = 1;

    texture_manager_ = TextureManager::get_instance();

    scale = 1.0f;

    diffuse_texture_ = diffuse == -1 ? texture_manager_->get_essential_texture(0) : texture_manager_->get_texture(diffuse);
    roughness_texture_ = roughness == -1 ? texture_manager_->get_essential_texture(0) : texture_manager_->get_texture(roughness);
    metallic_texture_ = metal == -1 ? texture_manager_->get_essential_texture(1) : texture_manager_->get_texture(metal);
    normal_texture_ = normal == -1 ? texture_manager_->get_essential_texture(0) : texture_manager_->get_texture(normal);

    cubemap_ = cubemap == -1 ? nullptr : texture_manager_->get_cubemap(cubemap);
}

TextureComponent::TextureComponent(Texture* diffuse, Texture* roughness, Texture* metal, Texture* normal, int type)
{
    this->type = type;
    if (type == GL_TEXTURE_2D) type_edit_ = 0;
    else type_edit_ = 1;

    texture_manager_ = TextureManager::get_instance();

    scale = 1.0f;

    diffuse_texture_ = diffuse ? diffuse : texture_manager_->get_essential_texture(0);
    roughness_texture_ = roughness ? roughness : texture_manager_->get_essential_texture(0);
    metallic_texture_ = metal ? metal : texture_manager_->get_essential_texture(1);
    normal_texture_ = normal ? normal : texture_manager_->get_essential_texture(0);

    cubemap_ = nullptr;
}


void TextureComponent::active_bind(const unsigned int offset)
{
    if (type == GL_TEXTURE_2D)
    {
        glActiveTexture(GL_TEXTURE0 + offset);
        glBindTexture(type, diffuse_texture_->get_handle());

        glActiveTexture(GL_TEXTURE1 + offset);
        glBindTexture(type, roughness_texture_->get_handle());

        glActiveTexture(GL_TEXTURE2 + offset);
        glBindTexture(type, metallic_texture_->get_handle());

        glActiveTexture(GL_TEXTURE3 + offset);
        glBindTexture(type, normal_texture_->get_handle());
    }
    else if (type == GL_TEXTURE_CUBE_MAP)
    {
        glActiveTexture(GL_TEXTURE0 + offset);
        glBindTexture(type, cubemap_->get_handle());
    }
}

bool TextureComponent::has_normal()
{
    return normal_texture_->get_handle() != texture_manager_->get_essential_texture(0)->get_handle();
}

bool TextureComponent::has_roughness()
{
    return roughness_texture_->get_handle() != texture_manager_->get_essential_texture(1)->get_handle();
}

bool TextureComponent::has_metallic()
{
    return metallic_texture_->get_handle() != texture_manager_->get_essential_texture(1)->get_handle();
}

std::string TextureComponent::get_name()
{
    return "Texture";
}


void TextureComponent::serialize(YAML::Emitter& out)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Texture" << YAML::Value << YAML::BeginMap;
    out << YAML::Key << "Enabled" << YAML::Value << is_enabled;

    out << YAML::Key << "Type" << YAML::Value << type;
    out << YAML::Key << "Scale" << YAML::Value << scale;
    if (type == GL_TEXTURE_2D)
    {
        out << YAML::Key << "Diffuse" << YAML::Value << diffuse_texture_->id;
        out << YAML::Key << "Roughness" << YAML::Value << roughness_texture_->id;
        out << YAML::Key << "Metallic" << YAML::Value << metallic_texture_->id;
        out << YAML::Key << "Normal" << YAML::Value << normal_texture_->id;
    }
    else if (type == GL_TEXTURE_CUBE_MAP)
    {
        out << YAML::Key << "Cubemap" << YAML::Value << cubemap_->faces[0]->id;
    }

    out << YAML::EndMap;
    out << YAML::EndMap;
}

bool TextureComponent::deserialize(YAML::Node& node)
{
    is_enabled = node["Enabled"].as<bool>();

    type = node["Type"].as<int>();
    scale = node["Scale"].as<float>();
    if (type == GL_TEXTURE_2D)
    {
        diffuse_texture_ = texture_manager_->get_texture_by_id(node["Diffuse"].as<unsigned int>());
        roughness_texture_ = texture_manager_->get_texture_by_id(node["Roughness"].as<unsigned int>());
        metallic_texture_ = texture_manager_->get_texture_by_id(node["Metallic"].as<unsigned int>());
        normal_texture_ = texture_manager_->get_texture_by_id(node["Normal"].as<unsigned int>());
    }
    else if (type == GL_TEXTURE_CUBE_MAP)
    {
        cubemap_ = texture_manager_->get_cubemap(0);
    }

    return true;
}
