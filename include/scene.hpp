// экраны игры
#pragma once
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "entity.hpp"
#include "engine.hpp"

class Engine;

class Scene {
  public:
    Scene(Engine* _engine, std::vector<const char*> entitynames);
    int W, H;
    // объекты сцены, первый всегда фон
    std::vector<Entity*> entities;
    virtual void reset() = 0;
    virtual void update(Uint64 dt);
    virtual void handleEvent(SDL_Event e) = 0;
    void createEntities(const std::vector<const char*>& entitynames);
    void (*loaderCallback)(Scene *) {nullptr};
    void loadTextures();
    Engine* engine;
};

// начальный экран
class WelcomeScreen : public Scene {
  public:
    WelcomeScreen(Engine* _engine);
    Entity start;
    void reset();
    void update(Uint64 dt);
    void handleEvent(SDL_Event e);     
};