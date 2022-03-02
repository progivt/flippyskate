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

Scene::~Scene() {
    while (!entities.empty()) {
        auto e = entities.back();
        entities.pop_back();
        if (e->name[0] == TXTMARK[0]) {
            SDL_Log("Destroying text %s", e->text.c_str());
            if (e->texture) {
                if (e->texture->sdlTexture) {
                    // почистить динамические текстуры для текста
                    SDL_DestroyTexture(e->texture->sdlTexture);
                    SDL_Log("Destroyed text texture for text \"%s\"", e->text.c_str());
                }
                delete e->texture;
            }
        } else
            SDL_Log("Destroying entity %s", e->name);
        delete e;
    }
    SDL_Log("Scene Destroyed");
}