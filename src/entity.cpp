#include <iostream>
#include "entity.hpp"

Sprite::Sprite(SDL_Renderer *_renderer, const char* path) {
	texture = IMG_LoadTexture(_renderer, path);
	// получить и запомнить размеры текстуры
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

Entity::Entity(SDL_Renderer *_renderer, Sprite _sprite,
	           float _px, float _py, float _vx, float _vy, float _ax, float _ay) 
               :
			   renderer {_renderer}, sprite{_sprite}, 
			   px{_px}, py{_py}, vx{_vx}, vy{_vy}, ax{_ax}, ay{_ay}, 
		 	   srcRect {0, 0, sprite.w, sprite.h} 
{
	SDL_GetRendererOutputSize(_renderer, &W, &H);
}

void Entity::tick(Uint64 dt){
	vx += ax * dt;
	vy += ay * dt;
	px += vx * dt;
	py += vy * dt;
}

void Entity::draw(){
	draw(px, py);
}

void Entity::draw(float _px, float _py){
	// рисование с x-обрезкой по ширине окна
	SDL_Rect srcRect {this->srcRect};
	SDL_Rect dstRect {(int)_px, (int)_py, srcRect.w, srcRect.h};
	int margin;
	if ((margin = dstRect.x + dstRect.w - W) > 0) {
		dstRect.w -= margin;
		srcRect.w -= margin;
	} 
	if ((margin = -dstRect.x) > 0) {
		dstRect.x = 0;
		dstRect.w -= margin;
		srcRect.x += margin;
		srcRect.w -= margin;
	}
	if (srcRect.w > 0)
		SDL_RenderCopy(renderer, sprite.texture, &srcRect, &dstRect);
}