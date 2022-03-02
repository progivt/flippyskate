// экраны игры
#pragma once
#include <vector>

#include <SDL2/SDL.h>

#include "entity.hpp"
#include "engine.hpp"

class Engine;

class Scene {
  public:
    Scene(Engine* _engine, std::vector<const char*> entitynames);
    ~Scene();
    int W, H;
    // объекты сцены, первый всегда фон
    std::vector<Entity*> entities;
    virtual void reset() = 0;
    virtual void update(Uint64 dt);
    virtual void handleEvent(SDL_Event e) = 0;
    void createEntities(const std::vector<const char*>& entitynames);
    void loadTextures();
    Engine* engine;
};

