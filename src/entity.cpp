#include "entity.hpp"
#include "engine.hpp"

Entity::Entity(const char *_imgFileName,
               float _px, float _py, 
               float _vx, float _vy, 
               float _ax, float _ay) 
    : px{_px}, py{_py}, 
      vx{_vx}, vy{_vy}, 
      ax{_ax}, ay{_ay},
	  srcRect {0, 0, 0, 0},
	  name {_imgFileName},
	  texture {0, 0, nullptr} {
	if (_imgFileName!= nullptr && _imgFileName[0] == TXTMARK) 
		text += _imgFileName;
}

// один временной шаг
void Entity::tick(Uint64 dt){
	vx += ax * dt;
	vy += ay * dt;
	px += vx * dt;
	py += vy * dt;
}

// движущийся фон зацикливается при выезде за пределы
void Background::tick(Uint64 dt) { 
	Entity::tick(dt); 
	if (px < -800) {
	    px = 0;
	}
}

