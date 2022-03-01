// основной игровой экран
#pragma once
#include "scene.hpp"

#define TEXTURE_RELOAD 10 // Код события, запрашивающего обновление показа очков

class GameScreen : public Scene {
  public:
    GameScreen(Engine* _engine);
    unsigned state, score, colIndex, nCols, nextColumn;
    Entity *bg, *player, *scorecard, *gameover, *finalbox, *yourscore, *highscore;
    void reset();
    void update(Uint64 dt);
    void handleEvent(SDL_Event e);
    static const int INTRO   = 0;
    static const int PLAYING = 1;
    static const int DYING   = 2;
    static const int DEAD    = 3;
  private:
    int maxy, miny;
    void startDeath();
    void respawnColumn(int i);
};
