#pragma once

#include <vector>
#include "imgui.h"


class Scene;
class Entity;

namespace editor
{
    class SceneGraphPanel
    {

        void draw_graph_children(Scene& scene, const std::vector<Entity*>& children, Entity*& selected);
    public:
        Entity* selected_entity = nullptr;

        void draw(Scene& scene);
    };
}
