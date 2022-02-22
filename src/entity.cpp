#include "entity.hpp"

Entity::Entity(SDL_Renderer *_renderer, const char*filename, 
	int _w, int _h, int _px, int _py, int _vx, int _vy, int _ax, int _ay) {
	px = _px;
	py = _py;
	vx = _vx;
	vy = _vy;
	ax = _ax;
	ay = _ay;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = _w;
	srcRect.h = _h;
	renderer = _renderer;
	tx = IMG_LoadTexture(renderer, filename);
	int _W, _H;
	//SDL_GetRendererOutputSize(_renderer, &_W, &_H);
}

void Entity::draw(){
	draw(px, py);
}

void Entity::draw(float _px, float _py){
	SDL_Rect dstRect {(int)_px, (int)_py, srcRect.w, srcRect.h};
	SDL_RenderCopy(renderer, tx, &srcRect, &dstRect);
}