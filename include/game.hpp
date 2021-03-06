// всеведущий мегакласс
#pragma once

#include "engine.hpp"
#include "scene.hpp"
#include "gamescreen.hpp"
#include "startscreen.hpp"
#include "scoresscreen.hpp"

class Game {
  public:
    Engine    engine;
    StartScreen startScreen;
    ScoresScreen scoresScreen;
    GameScreen gameScreen;
    std::vector<Scene *> scenes;
    Scene*    currentScene;
    Uint64    lastTime, lastDrawTime, ticks, frames;
    unsigned  highScore;

    Game();
    void run();

  private:
    void update();
    void invalidateScore();
    void repaint();
    unsigned lastScore;
};
