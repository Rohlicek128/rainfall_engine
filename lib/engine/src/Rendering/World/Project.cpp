#include "engine/world/Project.h"

#include "yaml-cpp/emittermanip.h"
#include "engine/core/Application.h"
#include "../Buffers/Textures/TextureManager.h"

#include <iostream>
#include <fstream>


namespace engine
{
    Project::Project(const std::string name)
    {
        this->name = name;
        this->save_path = "N/A";
        app_ = nullptr;
    }

    void Project::save_scene(Application& app, const std::string& name)
    {
        app.scene_manager->get_scene(name)->save(project_dir + scenes_dir + "\\" + name + ".rain");
    }

    void Project::save_scene(Scene& scene)
    {
        scene.save(project_dir + scenes_dir + "\\" + scene.name + ".rain");
    }


    void Project::save(Application& app, const std::string& file_path)
    {
        app_ = &app;
        YAML::Emitter out;

        serialize(out);

        std::ofstream file_out(file_path.c_str());
        file_out << out.c_str();

        save_path = file_path;
        std::cout << "[ENGINE] Project \"" << name << "\": SAVED to " << file_path << '\n';
    }

    bool Project::load(Application& app, const std::string& file_path)
    {
        app_ = &app;

        const std::ifstream stream(file_path);
        std::stringstream str_stream;
        str_stream << stream.rdbuf();

        YAML::Node project = YAML::Load(str_stream.str());

        app_->reset();
        const bool result = deserialize(project);

        save_path = file_path;
        std::cout << "[ENGINE] Project \"" << name << "\": LOADED from " << file_path << '\n';

        return result;
    }


    void Project::serialize(YAML::Emitter& out)
    {
        out << YAML::BeginMap;

        //Project
        out << YAML::Key << "Project" << YAML::BeginMap;
        out << YAML::Key << "Name" << YAML::Value << name.c_str();
        out << YAML::Key << "Version" << YAML::Value << "0.2.15";
        out << YAML::EndMap;

        //Directories
        out << YAML::Key << "Directories" << YAML::BeginMap;
        out << YAML::Key << "Root" << YAML::Value << project_dir.c_str();
        out << YAML::Key << "Assets" << YAML::Value << assets_dir.c_str();
        out << YAML::Key << "Scenes" << YAML::Value << scenes_dir.c_str();
        out << YAML::EndMap;

        //Scenes
        out << YAML::Key << "Scenes" << YAML::Value << YAML::BeginSeq;
        for (const auto & [key, value] : *app_->scene_manager->get_all_scenes())
        {
            value->save(value->save_path);
            out << YAML::Value << value->name + ".rain";
        }
        out << YAML::EndSeq;

        //Textures
        app_->resource_manager->get_texture_manager()->set_load_prefix(project_dir + assets_dir + "\\");
        app_->resource_manager->get_texture_manager()->serialize(out);

        //Models
        //out << YAML::Key << "Models" << YAML::Value << YAML::BeginSeq;
        //out << YAML::EndSeq;

        out << YAML::EndMap;
    }

    bool Project::deserialize(YAML::Node& node)
    {
        if (!node["Project"]) return false;

        name = node["Project"]["Name"].as<std::string>();

        project_dir = node["Directories"]["Root"].as<std::string>();
        assets_dir = node["Directories"]["Assets"].as<std::string>();
        scenes_dir = node["Directories"]["Scenes"].as<std::string>();

        app_->resource_manager->get_texture_manager()->set_load_prefix(project_dir + assets_dir + "\\");
        app_->resource_manager->get_texture_manager()->deserialize(node);

        if (YAML::Node scenes_des = node["Scenes"])
        {
            for (auto scene_des : scenes_des)
            {
                app_->scene_manager->load_scene(project_dir + scenes_dir + "\\" + scene_des.as<std::string>(), true);
            }
        }

        return true;
    }
}
