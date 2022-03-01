// экраны игры
#pragma once
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "entity.hpp"

#define SCORE_UPDATE 10 // Код события, запрашивающего обновление показа очков

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

// основной игровой экран
class Level : public Scene {
  public:
    Level(int _W, int _H);
    unsigned state, score, colIndex, numCols, nextColumn;
    Entity player, scorecard;
    void reset();
    void update(Uint64 dt);
    void handleEvent(SDL_Event e);
    static const int INTRO   = 0;
    static const int PLAYING = 1;
    static const int DEAD    = 2;
  private:
    int maxy, miny;
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