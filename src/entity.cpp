#include "entity.hpp"
#include "engine.hpp"

Entity::Entity(const char *_imgFileName,
               vec2 _pos, vec2 _v, vec2 _a)
  : pos{_pos}, v{_v}, a{_a},
	srcRect {0, 0, 0, 0},
	name {_imgFileName},
	texture {0, 0, nullptr} {
	// если текстовый объект, сначала просто копируем указанную 
	// строку в поле text за вычетом первого символа TXTMARK
	if (_imgFileName!= nullptr && _imgFileName[0] == TXTMARK) 
		text += _imgFileName;
}

// один временной шаг
void Entity::tick(Uint64 dt){
	v += dt*a;
	pos += dt*v;
}