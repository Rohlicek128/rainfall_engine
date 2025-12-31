#pragma once

#include <string>
#include "engine/core/ISerializable.h"


class Scene;

namespace engine
{
    class Application;

    class Project : ISerializable
    {
        Application* app_;

        void serialize(YAML::Emitter& out) override;
        bool deserialize(YAML::Node& node) override;
    public:
        std::string name;
        std::string save_path;

        std::string project_dir;
        std::string assets_dir;
        std::string scenes_dir;

        Project(const std::string name = "Untitled");

        void save(Application& app, const std::string& file_path);
        bool load(Application& app, const std::string& file_path);

        void save_scene(Application& app, const std::string& name);
        void save_scene(Scene& scene);
    };
}
