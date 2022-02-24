#include "gameplay.hpp"
#include "stdio.h"

void Background::tick(Uint64 dt) { 
	Entity::tick(dt); 
	printf("Btick\n");
	if (px < -screenW) {
	    px = 0;
	}
}

void Background::draw() { 
	// рисуем две копии 
	Entity::draw();
	Entity::draw(px + screenW, py);
}
