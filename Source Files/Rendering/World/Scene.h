#pragma once
#include <memory>
#include <vector>

#include "Mesh.h"

class Scene
{
public:
    std::string name;

    std::unique_ptr<Entity> root_entity;
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<Entity*> lights;
    std::unique_ptr<Mesh> mesh;

    Scene(const std::string&, Mesh&&);
    ~Scene() = default;

    void add_entity(std::unique_ptr<Entity>, bool = false);
    void add_light(Entity*);
    Entity* find_entity_by_id(unsigned int);
};
