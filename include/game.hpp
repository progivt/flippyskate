#pragma once
#include "engine.hpp"
#include "scene.hpp"

class Game {
  public:
    Engine 		engine;
    Scene			level, *currentScene;
    Uint64		lastTime, lastDrawTime, ticks, frames;
    int 			score, highScore;

    Game();
    void run();

  private:
    void update();
    void repaint();
  	void loadTextures(Scene *scene);
};

