#pragma once
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "entity.hpp"

class Scene {
  public:
    Scene(int _W, int _H);
    Background bg;
    std::vector<Entity*> entities;
    virtual void update(Uint64 dt);
    virtual void handleEvent(SDL_Event e);
    int W, H;
};

class Level : public Scene {
  public:
    Level(int _W, int _H);
    Entity player, col1, col2;
    virtual void update(Uint64 dt);
    virtual void handleEvent(SDL_Event e);
};
