#pragma once

#include "imgui.h"

#include <type_traits>
#include <engine/world/components/Component.h>
#include <engine/world/Entity.h>


class Entity;
class TextureManager;

namespace editor
{
    class EntityInspectorPanel
    {
        int add_selected_;
        char component_search_[64] = "";

        template<typename C>
        C* component_header(Entity* entity)
        {
            static_assert(std::is_base_of_v<Component, C>, "C must derive from Component");

            C* c = entity->get_component<C>();
            if (!c) return nullptr;

            ImGui::Checkbox(("##EnabledEC" + c->get_name()).c_str(), &c->is_enabled);
            ImGui::SameLine();

            bool is_opened = true;
            if (ImGui::CollapsingHeader((c->get_name() + " ##ComponentEC").c_str(), &is_opened, ImGuiTreeNodeFlags_DefaultOpen))
            {
                return c;
            }
            return nullptr;
        }

        bool check_search_string(const char*, const char*, int);

    public:
        EntityInspectorPanel();

        void draw(Entity* entity, TextureManager& manager);
    };
}
