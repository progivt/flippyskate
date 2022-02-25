#pragma once
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "entity.hpp"

class Scene {
  public:
    Scene(int _W, int _H);
    Entity player, col1, col2;
    std::vector<Entity*> entities;
    Background bg;
    virtual void update(Uint64 dt);
    virtual void handleEvent(SDL_Event e);
  private:
    int W, H;
};

