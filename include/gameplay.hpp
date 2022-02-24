#pragma once
#include "entity.hpp"

class Background : public Entity {
	using Entity::Entity;
  public:
  	void tick(Uint64 dt);
		void draw();
};