#pragma once
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "entity.hpp"

class Game {

public:
	Game(int width, int height, const char* windowTitle);
	~Game();
	SDL_Renderer *renderer;
    SDL_Window *window;
    std::vector<Sprite> sprites;
    std::vector<Entity> entities;

    void loadSprite(const char* path);
    void createEntity(Sprite sprite, float _px=0, float _py=0, 
                      float _vx=0, float _vy=0, float _ax=0, float _ay=0);
    void tick();
    void draw();
    Uint64 lastTime, lastDrawTime, ticks, frames;
    SDL_Event event;
};