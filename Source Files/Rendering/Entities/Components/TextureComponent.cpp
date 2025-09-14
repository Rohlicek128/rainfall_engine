#include "TextureComponent.h"

#include "../../Buffers/Textures/TextureManager.h"

TextureComponent::TextureComponent(const int type, const unsigned int diffuse, const unsigned int specular, const unsigned int normal, const int cubemap)
{
    this->type = type;
    if (type == GL_TEXTURE_2D) type_edit_ = 0;
    else type_edit_ = 1;

    texture_manager_ = TextureManager::get_instance();

    scale = 1.0f;
    
    diffuse_handle_ = diffuse;
    roughness_handle_ = specular;
    metallic_handle_ = 2;
    normal_handle_ = normal;
    if (cubemap == -1) cubemap_handle_ = texture_manager_->get_cubemap(0)->get_handle();
}

void TextureComponent::active_bind(const unsigned int offset)
{
    if (type == GL_TEXTURE_2D)
    {
        glActiveTexture(GL_TEXTURE0 + offset);
        glBindTexture(type, diffuse_handle_);

        glActiveTexture(GL_TEXTURE1 + offset);
        glBindTexture(type, roughness_handle_);

        glActiveTexture(GL_TEXTURE2 + offset);
        glBindTexture(type, metallic_handle_);

        glActiveTexture(GL_TEXTURE3 + offset);
        glBindTexture(type, normal_handle_);
    }
    else if (type == GL_TEXTURE_CUBE_MAP)
    {
        glActiveTexture(GL_TEXTURE0 + offset);
        glBindTexture(type, cubemap_handle_);
    }
}

int TextureComponent::has_normal()
{
    return normal_handle_ == 1 ? 0 : 1;
}

int TextureComponent::has_roughness()
{
    return roughness_handle_ == 2 ? 0 : 1;
}

int TextureComponent::has_metallic()
{
    return metallic_handle_ == 2 ? 0 : 1;
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
    texture_gui(&diffuse_handle_, "Diffuse");
    
    //Roughness
    ImGui::SeparatorText("Roughness Map");
    texture_gui(&roughness_handle_, "Roughness");
    if (roughness_handle_ == 1) roughness_handle_ = 2;

    //Metallic
    ImGui::SeparatorText("Metallic Map");
    texture_gui(&metallic_handle_, "Metallic");
    if (metallic_handle_ == 1) metallic_handle_ = 2;

    //Normal
    ImGui::SeparatorText("Normal Map");
    texture_gui(&normal_handle_, "Normal");
}

void TextureComponent::texture_gui(unsigned int* handle, const std::string& id_name)
{
    //Image Select
    Texture* texture = texture_manager_->get_texture_by_handle(*handle);
    if (texture->get_handle() == 1 || texture->get_handle() == 2)
    {
        if (ImGui::Button(("  Select..  ##" + id_name).c_str()))
            ImGui::OpenPopup(("Select " + id_name).c_str());
    }
    else
    {
        float image_size = 85.0f;
        if (ImGui::ImageButton(("Select ##" + id_name).c_str(), (ImTextureID)(intptr_t)*handle,
                               {image_size * ((float)texture->get_width() / (float)texture->get_height()),image_size},{0, 1}, {1, 0}))
            ImGui::OpenPopup(("Select " + id_name).c_str());
    }
    
    if (ImGui::BeginPopup(("Select " + id_name).c_str()))
    {
        const unsigned int selected = texture_manager_->select_texture_2d_gui();
        if (selected != 0) *handle = selected;
        ImGui::EndPopup();
    }

    //Info
    if (texture->get_handle() == 1 || texture->get_handle() == 2) return;
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
        ImGui::Text(texture->get_path().c_str());
        if (texture->get_handle() != 3)
        {
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
        out << YAML::Key << "Diffuse" << YAML::Value << diffuse_handle_;
        out << YAML::Key << "Roughness" << YAML::Value << roughness_handle_;
        out << YAML::Key << "Metallic" << YAML::Value << metallic_handle_;
        out << YAML::Key << "Normal" << YAML::Value << normal_handle_;
    }
    else if (type == GL_TEXTURE_CUBE_MAP)
    {
        out << YAML::Key << "Cubemap" << YAML::Value << cubemap_handle_;
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
        diffuse_handle_ = node["Diffuse"].as<unsigned int>();
        roughness_handle_ = node["Roughness"].as<unsigned int>();
        metallic_handle_ = node["Metallic"].as<unsigned int>();
        normal_handle_ = node["Normal"].as<unsigned int>();
    }
    else if (type == GL_TEXTURE_CUBE_MAP)
    {
        cubemap_handle_ = node["Cubemap"].as<unsigned int>();
    }
    
    return true;
}
