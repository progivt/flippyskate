// работа с SDL: рисование
#pragma once

#include <map>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "entity.hpp"

class Engine {
  public:
	  Engine(int width, int height);
    ~Engine();
	  
    SDL_Renderer *renderer;
    SDL_Window *window;
    std::map<const char*, Texture> images;
    
    Texture getTexture(const char* path);
    void loadEntityTexture(Entity* e);
    void draw(Entity* e);
    void draw(Entity* e, float _px, float _py);

  private:
    int W, H;
};