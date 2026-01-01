#include "ElementInspector.h"

#include "engine/src/Rendering/Buffers/Textures/TextureManager.h"
#include "engine/world/components/MeshComponent.h"
#include "imgui.h"
#include <engine/rendering/Framebuffer.h>
#include <engine/world/components/TransformComponent.h>
#include <engine/world/components/MaterialComponent.h>
#include <engine/world/components/TextureComponent.h>
#include <engine/world/components/MeshComponent.h>
#include <engine/world/components/CameraComponent.h>
#include <engine/world/components/LightComponent.h>
#include <engine/world/components/BehaviorComponent.h>

#include <engine/managers/Mesh.h>


namespace editor
{
    void ElementInspector::draw_framebuffer(Framebuffer& fbo)
    {
        if (fbo.attached_textures.at(0) == nullptr) return;

        ImGui::Image(
            (ImTextureID)(intptr_t)fbo.attached_textures.at(0)->get_handle(),
            {
                (float)fbo.attached_textures.at(0)->get_width(),
                (float)fbo.attached_textures.at(0)->get_height()
            },{0, 1}, {1, 0}
        );
    }

    void ElementInspector::draw_texture(Texture*& texture, const std::string& id_name, TextureManager& texture_manager)
    {
        //Image Select
        if (texture->id == texture_manager.get_essential_texture(0)->id ||
            texture->id == texture_manager.get_essential_texture(1)->id)
        {
            if (ImGui::Button(("  Select...  ##" + id_name).c_str()))
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
            if (Texture* selected = draw_texture_selection(texture_manager))
                texture = selected;
            ImGui::EndPopup();
        }

        //Info
        if (texture->id == texture_manager.get_essential_texture(0)->id || texture->id == texture_manager.get_essential_texture(1)->id)
            return;
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
            if (texture->id != texture_manager.get_essential_texture(2)->id)
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

    Texture* ElementInspector::draw_texture_selection(TextureManager& texture_manager)
    {
        ImGui::SeparatorText("Select a texture:");

        ImGui::SliderFloat("Scale", &texture_manager.select_scale, 30.0f, 400.0f, "%.0f");
        float max_width = 0.0;

        float width_aspect = 0.0f;
        for (int i = 0; i < texture_manager.get_texture_amount(); ++i)
        {
            width_aspect = (float)texture_manager.get_texture(i)->get_width() / (float)texture_manager.get_texture(i)->get_height();
            max_width += texture_manager.select_scale * width_aspect;
            //Image
            ImGui::PushID(i);
            if (ImGui::ImageButton("", (ImTextureID)(intptr_t)texture_manager.get_texture(i)->get_handle(),
                ImVec2(texture_manager.select_scale * width_aspect, texture_manager.select_scale), {0, 1}, {1, 0}))
            {
                ImGui::PopID();
                return texture_manager.get_texture(i);
            }
            ImGui::PopID();

            //Tooltip
            if (ImGui::BeginItemTooltip())
            {
                ImGui::Text(texture_manager.get_texture(i)->get_path().c_str());
                if (texture_manager.get_texture(i)->id != texture_manager.get_essential_texture(2)->id)
                {
                    ImGui::Text("Handle: %i", texture_manager.get_texture(i)->get_handle());
                    ImGui::Text("ID: %i", texture_manager.get_texture(i)->id);
                    ImGui::Text("Width: %i", texture_manager.get_texture(i)->get_width());
                    ImGui::Text("Height: %i", texture_manager.get_texture(i)->get_height());
                    ImGui::Text("Channels: %i", texture_manager.get_texture(i)->get_nr_channels());
                }
                else ImGui::Text("FAILED TO LOAD");


                ImGui::EndTooltip();
            }

            if (max_width <= 800.0 - texture_manager.select_scale) ImGui::SameLine();
            else max_width = 0.0;
        }
        ImGui::NewLine();
        if (ImGui::Button("None", ImVec2(120, 0))) return texture_manager.get_essential_texture(0);


        if (ImGui::Button("Add...", ImVec2(120, 0)))
            texture_manager.add_texture_open_file(texture_manager.load_as_srgb);

        ImGui::SameLine();
        ImGui::Checkbox("Load as SRGB", &texture_manager.load_as_srgb);

        return nullptr;
    }


    void ElementInspector::draw_vec3(const std::string& name, glm::vec3& vec, float speed, float min, float max)
    {
        float v[3] = {vec.x, vec.y, vec.z};
        ImGui::DragFloat3(name.c_str(), v, speed, min, max);
        vec.x = v[0];
        vec.y = v[1];
        vec.z = v[2];
    }

    void ElementInspector::draw_vec4(const std::string& name, glm::vec4& vec, float speed, float min, float max)
    {
        float v[4] = {vec.x, vec.y, vec.z, vec.w};
        ImGui::DragFloat4(name.c_str(), v, speed, min, max);
        vec.x = v[0];
        vec.y = v[1];
        vec.z = v[2];
        vec.w = v[3];
    }

    void ElementInspector::draw_color3(const std::string& name, glm::vec3& color)
    {
        float c[3] = {color.x, color.y, color.z};
        ImGui::ColorEdit3(name.c_str(), c);
        color.x = c[0];
        color.y = c[1];
        color.z = c[2];
    }

    void ElementInspector::draw_color4(const std::string& name, glm::vec4& color)
    {
        float c[4] = {color.x, color.y, color.z, color.w};
        ImGui::ColorEdit4(name.c_str(), c);
        color.x = c[0];
        color.y = c[1];
        color.z = c[2];
        color.w = c[3];
    }



    void ElementInspector::draw_transform_component(TransformComponent& transform)
    {
        draw_vec3("Position", transform.position, 0.01f);
        draw_vec3("Rotation", transform.rotation, 0.1f);
        draw_vec3("Scale", transform.scale, 0.01f);
    }

    void ElementInspector::draw_material_component(MaterialComponent& material)
    {
        draw_color4("Color ##Material", material.color);

        ImGui::SliderFloat("Roughness", &material.roughness, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Metallic", &material.metallic, 0.0f, 1.0f, "%.2f");
    }

    void ElementInspector::draw_texture_component(TextureComponent& texture, TextureManager& manager)
    {
        int type_edit = -1;
        if (texture.type == GL_TEXTURE_2D) type_edit = 0;
        else if (texture.type == GL_TEXTURE_CUBE_MAP) type_edit = 1;

        const char* type_names[] = { "Texture 2D", "Cubemap" };
        ImGui::Combo("Type", &type_edit, type_names, std::size(type_names));

        if (type_edit == 0) texture.type = GL_TEXTURE_2D;
        else if (type_edit == 1) texture.type = GL_TEXTURE_CUBE_MAP;


        //Scale
        ImGui::DragFloat("Scale", &texture.scale, 0.01f, 0, 0, "%.2f");

        //Diffuse
        ImGui::SeparatorText("Diffuse Map");
        draw_texture(texture.diffuse_texture, "Diffuse", manager);

        //Roughness
        ImGui::SeparatorText("Roughness Map");
        draw_texture(texture.roughness_texture, "Roughness", manager);
        if (texture.roughness_texture->id == manager.get_essential_texture(0)->id)
            texture.roughness_texture = manager.get_essential_texture(1);

        //Metallic
        ImGui::SeparatorText("Metallic Map");
        draw_texture(texture.metallic_texture, "Metallic", manager);
        if (texture.metallic_texture->id == manager.get_essential_texture(0)->id)
            texture.metallic_texture = manager.get_essential_texture(1);

        //Normal
        ImGui::SeparatorText("Normal Map");
        draw_texture(texture.normal_texture, "Normal", manager);
    }

    void ElementInspector::draw_mesh_component(MeshComponent& mesh)
    {
        ImGui::SeparatorText("Model");
        if (const ModelData* model_data = mesh.mesh->get_model(mesh.model_index))
        {
            if (ImGui::Selectable(("  " + model_data->name).c_str(), true, 0, {ImGui::GetContentRegionAvail().x * 0.5f, 0}))
                mesh.is_mesh_view_open = true;
            ImGui::SetItemTooltip("Click to Browse");
        }
        //ImGui::SameLine();
        //if (ImGui::Button("Change..")) is_mesh_view_open_ = true;

        ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
        if (mesh.is_mesh_view_open)
        {
            if (ImGui::Begin("Select a Mesh", &mesh.is_mesh_view_open, ImGuiWindowFlags_NoDocking))
            {
                const int selected = -1; //mesh.mesh->select_mesh_gui();
                if (selected != -1)
                {
                    mesh.model_index = selected;
                    mesh.is_mesh_view_open = false;
                }
            }
            ImGui::End();
        }

        ImGui::SeparatorText("Rendering");
        //ImGui::Checkbox("Is Shaded", &is_shaded);
        ImGui::Checkbox("Is Culled", &mesh.is_culled);
        ImGui::Checkbox("Is Inverted", &mesh.is_inverted);

        const char* type_names[] = { "Points", "Lines", "Line Loop", "Line Strip", "Triangles", "Triangle Strip", "Triangle Fan", "Quads", "Quads Strip", "Polygon" };
        ImGui::Combo("Primitive", &mesh.primitive_type, type_names, std::size(type_names));
    }

    void ElementInspector::draw_camera_component(CameraComponent& camera)
    {
        ImGui::SeparatorText("Rendering");
        ImGui::Checkbox("Wireframe", &camera.is_wireframe);
        ImGui::ColorEdit4("Background", camera.clear_color);

        ImGui::SeparatorText("Tone Mapping");
        ImGui::SliderFloat("Gamma", &camera.gamma, 0.0f, 10.0f, "%.1f");
        ImGui::SliderFloat("Threshold", &camera.threshold, 0.0f, 15.0f, "%.2f");
        ImGui::SliderFloat("Key Value", &camera.key_value, 0.0f, 10.0f, "%.3f");


        ImGui::SeparatorText("Rotation");
        ImGui::PushItemWidth(150);
        ImGui::DragFloat("Yaw", &camera.yaw, 0.1f);
        ImGui::SameLine();
        ImGui::DragFloat("Pitch", &camera.pitch, 0.1f);
        ImGui::PopItemWidth();
        camera.set_yaw_pitch(camera.yaw, camera.pitch);

        ImGui::SeparatorText("Field of View");
        ImGui::SliderFloat("##Fov", &camera.fov, 10.0f, 125.0f, "%.1f");

        ImGui::SeparatorText("Clipping Planes");
        ImGui::DragFloat("Near Plane", &camera.near_plane, 0.01f, 0.01f, 100.0f);
        ImGui::DragFloat("Far Plane", &camera.far_plane, 10.0f, 10.0f, 1000000.0f);

        ImGui::SeparatorText("Movement");
        ImGui::DragFloat("Speed", &camera.speed, 0.1f, 0.1f, 500.0f);
    }

    void ElementInspector::draw_light_component(LightComponent& light)
    {
        int type_edit = static_cast<int>(light.type);
        const char* type_names[] = { "Directional", "Point", "Spotlight" };
        ImGui::Combo("Type", &type_edit, type_names, std::size(type_names));
        light.type = (lights::LIGHT_TYPE)type_edit;

        ImGui::SeparatorText("Color");

        draw_color3("Color ##Light", light.color);


        if (ImGui::Button("R")) light.intensity = 1.0f;
        ImGui::SameLine();
        ImGui::DragFloat("Intensity", &light.intensity, 0.01f, 0, 0, "%.1f");
        light.intensity = std::max(light.intensity, 0.0f);

        if (light.type == lights::LIGHT_TYPE::POINT)
        {
            ImGui::SeparatorText("Attenuation");
            ImGui::PushItemWidth(200);
            ImGui::DragFloat("Constant", &light.attenuation_params.x, 0.01f, 0, 0, "%.4f");
            ImGui::DragFloat("Linear", &light.attenuation_params.y, 0.001f, 0, 0, "%.4f");
            ImGui::DragFloat("Quadratic", &light.attenuation_params.z, 0.0001f, 0, 0, "%.4f");
            ImGui::PopItemWidth();
        }
    }

    void ElementInspector::draw_behavior_component(BehaviorComponent& behavior)
    {
        ImGui::Text("Name: ");
        ImGui::SameLine();
        ImGui::Text(behavior.name.c_str());
    }
}
