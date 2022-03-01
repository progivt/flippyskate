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
	  
    int W, H;
    SDL_Renderer *renderer;
    SDL_Window *window;
    TTF_Font *font;
    std::map<const char*, Texture> images;
    
    void loadEntityTexture(Entity* e);
    void setFontSize(int size=0);
    void draw(Entity* e);
    void draw(Entity* e, vec2 pos);
    void draw(Entity* e, int x, int y, int w, int h);
};