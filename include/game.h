#pragma once

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <glad/glad.h>

class Game {

public:
	Game(int width, int height, const char* windowTitle);
	~Game();
	SDL_Renderer *renderer;
    SDL_Window *window;

private:
    SDL_Event event;
};