#pragma once

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <glad/glad.h>

#include "entity.hpp"

class Game {

public:
	Game(int width, int height, const char* windowTitle);
	~Game();
	SDL_Renderer *renderer;
    SDL_Window *window;
    std::vector<Sprite> sprites;
    std::vector<Entity> entities;
    void LoadSprite(const char* path);
    void CreateEntity(Sprite sprite, float _px=0, float _py=0, 
                      float _vx=0, float _vy=0, float _ax=0, float _ay=0);
    void Tick(Uint64 dt);

private:
    SDL_Event event;
};