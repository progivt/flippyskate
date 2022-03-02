// основной игровой экран
#pragma once
#include "scene.hpp"

#define GO_START   101

class GameScreen : public Scene {
  public:
    GameScreen(Engine* _engine);
    unsigned state, score, maxScore, col0, nCols, ovrl0, nextColumn;
    Entity *bg, *player, *scorecard, *gameover, *finalbox, *yourscore, 
           *highscore, *medal;
    void reset();
    void update(Uint64 dt);
    void handleEvent(SDL_Event e);
    static const int INTRO   = 0;
    static const int PLAYING = 1;
    static const int DYING   = 2;
    static const int DEAD    = 3;
  private:
    int maxy, miny;
    void displayOverlays(bool show);
    void overlayReset();
    void startDeath();
    void respawnColumn(int i);
};
