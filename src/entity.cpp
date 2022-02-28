#include "entity.hpp"
#include "engine.hpp"

Entity::Entity(const char *_imgFileName,
               vec2 _pos, vec2 _v, vec2 _a)
  : pos{_pos}, v{_v}, a{_a},
	srcRect {0, 0, 0, 0},
	name {_imgFileName},
	texture {0, 0, nullptr} {
	if (_imgFileName!= nullptr && _imgFileName[0] == TXTMARK) 
		text += _imgFileName;
}

// один временной шаг
void Entity::tick(Uint64 dt){
	v += dt*a;
	pos += dt*v;
}

// движущийся фон зацикливается при выезде за пределы
void Background::tick(Uint64 dt) { 
	Entity::tick(dt); 
	if (pos.x < -800) {
	    pos.x = 0;
	}
}

