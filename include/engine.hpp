#pragma once
#include <vector>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "entity.hpp"
#include "gameplay.hpp"

#define JUMP_ACCEL 0.23f

class Engine {
  public:
	  Engine(int width, int height);
    ~Engine();
    bool exiting { false  };
	  
    SDL_Renderer *renderer;
    SDL_Window *window;
    std::vector<Sprite> sprites;
    std::vector<Entity*> entities;
    
    void loadSprite(const char* path);
    void update();
    void repaint();
    void handleEvent(SDL_Event e);
  private:
    Entity player, col1, col2;
    Background bg;
    Uint64 lastTime, lastDrawTime, ticks, frames;    
    int W, H;

    void initSDL(int width, int height);
};