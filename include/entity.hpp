#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// текстура из файла и ее размеры
class Sprite {
public:
	Sprite(SDL_Renderer *_renderer, const char* path);
	int w, h;
	SDL_Texture *texture;
};

// объект, отрисовываемый из текстуры, с вещественными положением, 
// скоростью и ускорением
class Entity {
public:
	float px, py, vx, vy, ax, ay;
	int W, H;
	Entity(SDL_Renderer *_renderer, Sprite sprite,
		   float _px=0, float _py=0, float _vx=0, float _vy=0, float _ax=0, float _ay=0);
	void Tick(Uint64 dt);
	void draw();
	void draw(float _px, float _py);
private:
	SDL_Renderer *renderer;
	Sprite sprite;
	
	SDL_Rect srcRect;
};