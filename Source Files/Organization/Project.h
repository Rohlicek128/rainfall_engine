#pragma once
#include <memory>
#include <string>
#include <vector>

#include "../Rendering/World/Loading/ISerializable.h"

class Scene;
class TextureManager;
class Mesh;

class Project : ISerializable
{
    void serialize(YAML::Emitter& out) override;
    bool deserialize(YAML::Node& node) override;
public:
    std::string name;
    std::string path;
    
    std::vector<std::unique_ptr<Scene>> scenes;
    Scene* current_scene;

    std::unique_ptr<Mesh> assets_mesh;
    TextureManager* textures;

    Project(const std::string& name);

    void save(const std::string& file_path);
    bool load(const std::string& file_path);
    void add_empty_scene();
    void reset();
    
    bool load_scene_from_file();
    bool load_scene_from_path(const std::string& file_path);
};
