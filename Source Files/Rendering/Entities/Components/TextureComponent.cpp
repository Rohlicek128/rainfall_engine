#include "TextureComponent.h"

#include <glad/glad.h>

TextureComponent::TextureComponent(unsigned int diffuse, unsigned int specular)
{
    diffuse_handle_ = diffuse;
    specular_handle_ = specular;

    texture_manager_ = TextureManager::get_instance();
}

void TextureComponent::active_bind(const unsigned int index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, diffuse_handle_);

    glActiveTexture(GL_TEXTURE1 + index);
    glBindTexture(GL_TEXTURE_2D, specular_handle_);
}

void TextureComponent::set_gui()
{
    //Diffuse
    ImGui::SeparatorText("Diffuse Map");
    texture_gui(&diffuse_handle_, "Diffuse");
    
    //Specular
    ImGui::SeparatorText("Specular Map");
    texture_gui(&specular_handle_, "Specular");
}

void TextureComponent::texture_gui(unsigned int* handle, const std::string& id_name)
{
    //Image Select
    Texture* texture = texture_manager_->get_texture_by_handle(*handle);
    if (texture->get_handle() == 1)
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
        const unsigned int selected = texture_manager_->select_texture_gui();
        if (selected != 0) *handle = selected;
        ImGui::EndPopup();
    }

    //Info
    if (texture->get_handle() == 1) return;
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