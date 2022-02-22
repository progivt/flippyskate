#pragma once

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

class Entity {
public:
	float px, py, vx, vy, ax, ay;
	Entity(SDL_Renderer *_renderer, const char*filename, int _w, int _h, 
		   int _px=0, int _py=0, int _vx=0, int _vy=0, int _ax=0, int _ay=0);
	void draw();
	void draw(float _px, float _py);
private:
	SDL_Renderer *renderer;
	SDL_Texture *tx;
	SDL_Rect srcRect;
};