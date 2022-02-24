#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

// текстура из файла и ее размеры
struct Texture {
  int w, h;
  SDL_Texture *texture;
};

// объект, отрисовываемый из текстуры, с вещественными положением, 
// скоростью и ускорением
class Entity {
public:
	float px, py, vx, vy, ax, ay;
	int screenW, screenH;
	Entity(SDL_Renderer *_renderer=nullptr, Texture _texture = Texture{ 0, 0, NULL },
		   float _px=0, float _py=0, float _vx=0, float _vy=0, float _ax=0, float _ay=0);
	virtual void tick(Uint64 dt);
	virtual void draw();
	void draw(float _px, float _py);
	SDL_Rect srcRect;
private:
	SDL_Renderer *renderer;
	SDL_Texture *texture;
};