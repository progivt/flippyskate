// всеведущий мегакласс
#pragma once

#include <sstream>

#include "engine.hpp"
#include "scene.hpp"

class Game {
  public:
    Engine 		engine;
    Level     level;
    WelcomeScreen welcomeScreen;
    Scene*		currentScene;
    Uint64		lastTime, lastDrawTime, ticks, frames;
    unsigned 	highScore;

    Game();
    void run();

  private:
    void update();
    void invalidateScore();
    void repaint();
  	void loadTextures(Scene *scene);
  	std::ostringstream strScore;
  	unsigned lastScore;
};
        