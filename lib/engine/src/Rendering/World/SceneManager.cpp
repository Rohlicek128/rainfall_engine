#include "engine/managers/SceneManager.h"

#include <memory>


namespace engine
{
    SceneManager::SceneManager(ResourceManager& resource_manager)
    {
        resource_manager_ = &resource_manager;
        current_scene_ = nullptr;
    }

    Scene* SceneManager::get_current_scene()
    {
        return current_scene_;
    }

    Scene* SceneManager::get_scene(const std::string& name)
    {
        if (!scenes_.contains(name)) return nullptr;

        return scenes_.at(name).get();
    }

    bool SceneManager::switch_to(const std::string& name)
    {
        Scene* desired_scene = get_scene(name);

        if (desired_scene)
        {
            current_scene_ = desired_scene;
            return true;
        }
        return false;
    }

    bool SceneManager::load_scene(const std::string& path, bool make_current)
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>("Temp");
        if (scene->load(path))
        {
            scene->set_mesh(*resource_manager_->get_mesh_manager());

            const std::string& name = scene->name;
            auto result = scenes_.emplace(name, std::move(scene));
            if (make_current && result.second) switch_to(name);

            return result.second;
        }
        return false;
    }

    bool SceneManager::load_scene(std::unique_ptr<Scene> scene, bool make_current)
    {
        if (!scene) return false;

        scene->set_mesh(*resource_manager_->get_mesh_manager());

        const std::string& name = scene->name;
        auto result = scenes_.emplace(name, std::move(scene));
        if (make_current && result.second) switch_to(name);

        return result.second;
    }

    Scene* SceneManager::create_scene(const std::string& name, bool make_current)
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>(name);
        scene->set_mesh(*resource_manager_->get_mesh_manager());

        auto result = scenes_.emplace(name, std::move(scene));
        if (make_current && result.second) switch_to(name);

        return get_scene(name);
    }
}
