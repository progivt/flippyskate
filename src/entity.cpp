#include "entity.hpp"
#include "stdio.h"

Entity::Entity(SDL_Renderer* _renderer, Texture _texture,
	           float _px, float _py, float _vx, float _vy, float _ax, float _ay) 
               :
			   renderer {_renderer}, texture{_texture.texture}, 
			   px{_px}, py{_py}, vx{_vx}, vy{_vy}, ax{_ax}, ay{_ay},
			   srcRect {0, 0, 0, 0} 
{
	if (renderer!=nullptr)
		SDL_GetRendererOutputSize(renderer, &screenW, &screenH);
	srcRect.w = _texture.w;
	srcRect.h = _texture.h;
}

// один временной шаг
void Entity::tick(Uint64 dt){
	printf("Etick\n");
	vx += ax * dt;
	vy += ay * dt;
	px += vx * dt;
	py += vy * dt;
}

// рисование в текущих координатах
void Entity::draw(){
	draw(px, py);
}

// рисование с x-обрезкой по ширине окна в произвольных координатах
void Entity::draw(float _px, float _py){
	SDL_Rect srcRect {this->srcRect};
	SDL_Rect dstRect {(int)_px, (int)_py, srcRect.w, srcRect.h};
	int margin;
	if ((margin = dstRect.x + dstRect.w - screenW) > 0) {
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
		SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}