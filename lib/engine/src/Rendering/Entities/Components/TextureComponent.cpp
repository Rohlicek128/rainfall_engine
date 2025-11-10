#include "TextureComponent.h"

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

void TextureComponent::set_gui()
{
    const char* type_names[] = { "Texture 2D", "Cubemap" };
    ImGui::Combo("Type", &type_edit_, type_names, std::size(type_names));
    if (type_edit_ == 0) type = GL_TEXTURE_2D;
    else if (type_edit_ == 1) type = GL_TEXTURE_CUBE_MAP;

    //Scale
    ImGui::DragFloat("Scale", &scale, 0.01f, 0, 0, "%.2f");
    
    //Diffuse
    ImGui::SeparatorText("Diffuse Map");
    texture_gui(diffuse_texture_, "Diffuse");
    
    //Roughness
    ImGui::SeparatorText("Roughness Map");
    texture_gui(roughness_texture_, "Roughness");
    if (roughness_texture_->id == texture_manager_->get_essential_texture(0)->id)
        roughness_texture_ = texture_manager_->get_essential_texture(1);

    //Metallic
    ImGui::SeparatorText("Metallic Map");
    texture_gui(metallic_texture_, "Metallic");
    if (metallic_texture_->id == texture_manager_->get_essential_texture(0)->id)
        metallic_texture_ = texture_manager_->get_essential_texture(1);

    //Normal
    ImGui::SeparatorText("Normal Map");
    texture_gui(normal_texture_, "Normal");
}

void TextureComponent::texture_gui(Texture*& texture, const std::string& id_name)
{
    //Image Select
    if (texture->id == texture_manager_->get_essential_texture(0)->id ||
        texture->id == texture_manager_->get_essential_texture(1)->id)
    {
        if (ImGui::Button(("  Select..  ##" + id_name).c_str()))
            ImGui::OpenPopup(("Select " + id_name).c_str());
    }
    else
    {
        float image_size = 85.0f;
        if (ImGui::ImageButton(("Select ##" + id_name).c_str(), (ImTextureID)(intptr_t)texture->get_handle(),
                               {image_size * ((float)texture->get_width() / (float)texture->get_height()),image_size},
                               {0, 1}, {1, 0}))
            ImGui::OpenPopup(("Select " + id_name).c_str());
    }
    
    if (ImGui::BeginPopup(("Select " + id_name).c_str()))
    {
        if (Texture* selected = texture_manager_->select_texture_2d_gui())
            texture = selected;
        ImGui::EndPopup();
    }

    //Info
    if (texture->id == texture_manager_->get_essential_texture(0)->id || texture->id == texture_manager_->get_essential_texture(1)->id)
        return;
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
        if (texture->id != texture_manager_->get_essential_texture(2)->id)
        {
            ImGui::Text("[%i] %s", texture->id, texture->get_path().c_str());
            ImGui::Text("Width: %i", texture->get_width());
            ImGui::Text("Height: %i", texture->get_height());
            //ImGui::Text("Channels: %i", texture->get_nr_channels());
        }
        else ImGui::Text("FAILED TO LOAD");
        ImGui::EndGroup();
    }
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
