#pragma once

#include <sstream>

#include "engine.hpp"
#include "scene.hpp"

class Game {
  public:
    Engine 		engine;
    Level     level;
    Scene*		currentScene;
    Uint64		lastTime, lastDrawTime, ticks, frames;
    int 			score, highScore;

    Game();
    void run();

  private:
    void update();
    void repaint();
  	void loadTextures(Scene *scene);
  	std::ostringstream strScore;
};

