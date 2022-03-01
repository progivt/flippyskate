#include "entity.hpp"
#include "engine.hpp"

Entity::Entity(const char *name,
               vec2 _pos, vec2 _v, vec2 _a)
  : pos{_pos}, v{_v}, a{_a},
	srcRect {0, 0, 0, 0},
	name {name},
	texture {nullptr} {
	// если текстовый объект, сначала просто копируем указанную 
	// строку в поле text за вычетом первого символа TXTMARK
	if (name!= nullptr && name[0] == TXTMARK[0]) 
		text += name + 1;
}

// один временной шаг
void Entity::tick(Uint64 dt){
	v += dt*a;
	pos += dt*v;
}