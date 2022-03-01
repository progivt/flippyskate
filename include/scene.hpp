// экраны игры
#pragma once
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "entity.hpp"


class Scene {
  public:
    Scene(int _W, int _H);
    int W, H;
    Entity bg;
    std::vector<Entity*> entities;
    virtual void reset() = 0;
    virtual void update(Uint64 dt);
    virtual void handleEvent(SDL_Event e) = 0;
};

// начальный экран
class WelcomeScreen : public Scene {
  public:
    WelcomeScreen(int _W, int _H);
    Entity start;
    void reset();
    void update(Uint64 dt);
    void handleEvent(SDL_Event e);     
};