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
    Background bg;
    std::vector<Entity*> entities;
    virtual void update(Uint64 dt);
    virtual void handleEvent(SDL_Event e);
};

// основной игровой экран
class Level : public Scene {
  public:
    Level(int _W, int _H);
    Entity player, scorecard, col1, col2;
    void update(Uint64 dt);
    void handleEvent(SDL_Event e);
};

// начальный экран
class WelcomeScreen : public Scene {
  public:
    WelcomeScreen(int _W, int _H);
    Entity start;
    void update(Uint64 dt);
    void handleEvent(SDL_Event e);     
};