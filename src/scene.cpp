#include "scene.hpp"

Scene::Scene(Engine* _engine, std::vector<const char*> entitynames) 
  : W{_engine->W}, H{_engine->H}, engine{_engine} {
    createEntities(entitynames);
}

void Scene::createEntities(const std::vector<const char*>& entitynames){
    for (auto& name: entitynames)
        entities.push_back(new Entity{name});
}

void Scene::update(Uint64 dt) {   
    for (auto& e : entities){
        e->tick(dt);
        if (e == entities[0] && e->pos.x < -W) // заворачиваем фон по кругу 
            e->pos.x = 0;
    }
}

void Scene::loadTextures(){
    for (auto& e : entities)
        engine->loadEntityTexture(e);
}
