#include "Project.h"

#include <iostream>
#include <fstream>

#include "../Rendering/World/Scene.h"
#include "../Rendering/World/Mesh.h"
#include "../Rendering/Buffers/Textures/TextureManager.h"

#include "../Utils/FileDialogs.h"

Project::Project(const std::string& name)
{
    this->name = name;
    this->path = "N/A";

    current_scene = nullptr;

    assets_mesh = std::make_unique<Mesh>(std::vector<VertexAttribute>{{"Position", 3}, {"TexCoord", 2}, {"Normal", 3}}, true);
    textures = TextureManager::get_instance();
}

void Project::add_empty_scene()
{
    scenes.push_back(std::make_unique<Scene>("Untitled", assets_mesh.get()));
    current_scene = scenes.back().get();
}

void Project::reset()
{
    scenes.clear();
    current_scene = nullptr;
    name = "Untitled";
    path = "N/A";
}

bool Project::load_scene_dialog()
{
    const std::string open_path = FileDialogs::open_file("Rainfall Scene (*.rain)\0*.rain\0");
    if (!open_path.empty())
        load_scene_from_path(open_path);
    return false;
}

bool Project::load_scene_from_path(const std::string& file_path)
{
    scenes.push_back(std::make_unique<Scene>("Untitled", assets_mesh.get()));
    if (!scenes.back()->load(file_path))
    {
        scenes.erase(scenes.begin() + scenes.size() - 1);
        return false;
    }
    current_scene = scenes.back().get();
    return true;
}


void Project::save(const std::string& file_path)
{
    YAML::Emitter out;
    
    serialize(out);
    
    std::ofstream file_out(file_path.c_str());
    file_out << out.c_str();

    this->path = file_path;
    std::cout << "SAVED PROJECT: " << file_path << '\n';
}

bool Project::load(const std::string& file_path)
{
    const std::ifstream stream(file_path);
    std::stringstream str_stream;
    str_stream << stream.rdbuf();

    YAML::Node project = YAML::Load(str_stream.str());

    reset();
    const bool result = deserialize(project);

    this->path = file_path;
    std::cout << "LOADED PROJECT: " << file_path << '\n';

    return result;
}

void Project::serialize(YAML::Emitter& out)
{
    out << YAML::BeginMap;

    out << YAML::Key << "Project" << YAML::Value << name.c_str();
    
    out << YAML::Key << "Scene paths" << YAML::Value << YAML::BeginSeq;
    for (int i = 0; i < scenes.size(); ++i)
        out << YAML::Value << scenes.at(i)->save_path;
    out << YAML::EndSeq;

    textures->serialize(out);
    
    out << YAML::EndMap;
}

bool Project::deserialize(YAML::Node& node)
{
    if (!node["Project"]) return false;
    
    name = node["Project"].as<std::string>();

    textures->deserialize(node);

    if (YAML::Node scenes_des = node["Scene paths"])
    {
        for (auto scene_des : scenes_des)
        {
            load_scene_from_path(scene_des.as<std::string>());
        }
    }
    
    return true;
}