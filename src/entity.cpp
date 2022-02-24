#include "entity.hpp"


Sprite::Sprite(SDL_Renderer *_renderer, const char* path) {
	if (path !=nullptr && _renderer != nullptr) {
		texture = IMG_LoadTexture(_renderer, path);
		// получить и запомнить размеры текстуры
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	}
}

Entity::Entity(SDL_Renderer* _renderer, Sprite* _sprite,
	           float _px, float _py, float _vx, float _vy, float _ax, float _ay) 
               :
			   renderer {_renderer}, sprite{_sprite}, 
			   px{_px}, py{_py}, vx{_vx}, vy{_vy}, ax{_ax}, ay{_ay},
			   srcRect {0, 0, 0, 0} 
{
	if (renderer!=nullptr)
		SDL_GetRendererOutputSize(renderer, &screenW, &screenH);
	if (_sprite!=nullptr){
		srcRect.w = sprite->w;
		srcRect.h = sprite->h;
	}
}

// один временной шаг
void Entity::tick(Uint64 dt){
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
		SDL_RenderCopy(renderer, sprite->texture, &srcRect, &dstRect);
}