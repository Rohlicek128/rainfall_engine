#pragma once

#include "glm/fwd.hpp"
#include <string>


class Framebuffer;
class Texture;
class TextureManager;

class TransformComponent;
class CameraComponent;
class MaterialComponent;
class MeshComponent;
class LightComponent;
class TextureComponent;

namespace editor
{
    class ElementInspector
    {
    public:
        static void draw_framebuffer(Framebuffer& fbo);
        static void draw_texture(Texture*& texture, const std::string& id_name, TextureManager& texture_manager);
        static Texture* draw_texture_selection(TextureManager& texture_manager);

        static void draw_vec3(const std::string& name, glm::vec3& vec, float speed = 1.f, float min = 0.f, float max = 0.f);
        static void draw_vec4(const std::string& name, glm::vec4& vec, float speed = 1.f, float min = 0.f, float max = 0.f);
        static void draw_color3(const std::string& name, glm::vec3& color);
        static void draw_color4(const std::string& name, glm::vec4& color);

        static void draw_transform_component(TransformComponent& transform);
        static void draw_mesh_component(MeshComponent& mesh);
        static void draw_camera_component(CameraComponent& camera);
        static void draw_material_component(MaterialComponent& material);
        static void draw_texture_component(TextureComponent&, TextureManager& manager);
        static void draw_light_component(LightComponent& light);
    };
}
