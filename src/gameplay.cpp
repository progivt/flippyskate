#include "gameplay.hpp"

void Background::tick(Uint64 dt) { 
	Entity::tick(dt); 
	if (px < -screenW) {
	    px = 0;
	}
}

void Background::draw() { 
	// рисуем две копии 
	Entity::draw();
	Entity::draw(px + screenW, py);
}
