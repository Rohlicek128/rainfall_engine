#include "Scene.h"

Scene::Scene(const std::string& name, Mesh&& mesh)
{
    root_entity = std::make_unique<Entity>("__root", new TransformComponent(glm::vec3(0.0f), glm::vec3(0.0f) ,glm::vec3(0.0f)));
    root_entity->is_root = true;
    
    this->name = name;
    this->mesh = std::make_unique<Mesh>(std::move(mesh));
}

void Scene::add_entity(std::unique_ptr<Entity> entity, const bool is_light)
{
    if (entity->parent == nullptr) root_entity->add_child(entity.get());
    entities.push_back(std::move(entity));
    if (is_light) add_light(entities.back().get());
}

void Scene::add_light(Entity* light)
{
    lights.push_back(light);
}

Entity* Scene::find_entity_by_id(const unsigned int id)
{
    for (int i = 0; i < entities.size(); ++i)
    {
        if (entities.at(i)->id == id) return entities.at(i).get();
    }
    return nullptr;
}
