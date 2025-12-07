#pragma once

#include "engine/managers/ResourceManager.h"
#include "engine/world/Scene.h"
#include <memory>
#include <string>
#include <unordered_map>

class Mesh;

namespace engine
{
    class ResourceManager;

    class SceneManager
    {
        ResourceManager* resource_manager_;

        std::unordered_map<std::string, std::unique_ptr<Scene>> scenes_;
        Scene* current_scene_;

    public:
        SceneManager(ResourceManager& resource_manager);

        Scene* get_current_scene();

        bool switch_to(const std::string& name);
        bool load_scene(const std::string& path, bool make_current = false);
        bool load_scene(std::unique_ptr<Scene> scene, bool make_current);
    };
}
