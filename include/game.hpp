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
    void LoadSprite(const char* path);

private:
    SDL_Event event;
};