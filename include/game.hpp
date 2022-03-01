// всеведущий мегакласс
#pragma once

#include <sstream>

#include "engine.hpp"
#include "scene.hpp"
#include "gamescreen.hpp"

class Game {
  public:
    Engine 		engine;
    WelcomeScreen welcomeScreen;
    GameScreen gameScreen;
  	std::vector<Scene *> scenes;
    Scene*		currentScene;
    Uint64		lastTime, lastDrawTime, ticks, frames;
    unsigned 	highScore;

    Game();
    ~Game();
    void run();

  private:
    void update();
    void invalidateScore();
    void repaint();
  	std::ostringstream strScore;
  	unsigned lastScore;
};
        