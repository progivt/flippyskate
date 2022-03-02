// всеведущий мегакласс
#pragma once

#include <sstream>

#include "engine.hpp"
#include "scene.hpp"
#include "gamescreen.hpp"
#include "startscreen.hpp"

class Game {
  public:
    Engine 		engine;
    StartScreen startScreen;
    GameScreen gameScreen;
  	std::vector<Scene *> scenes;
    Scene*		currentScene;
    Uint64		lastTime, lastDrawTime, ticks, frames;
    unsigned 	highScore;

    Game();
    void run();

  private:
    void update();
    void invalidateScore();
    void repaint();
  	std::ostringstream strScore;
  	unsigned lastScore;
};
        