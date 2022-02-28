// объект с текстурой либо отрисованным текстом, 
// с вещественными  положением, скоростью и ускорением
#pragma once
#include <iostream>
#include <SDL2/SDL.h>

#include "util.hpp"
#define TXTMARK '*'

class Entity {
  public:
    vec2 pos, v, a;
    SDL_Rect srcRect;
    Entity(const char *_imgFileName=nullptr,
           vec2 _pos=vec2 {0, 0},
           vec2 _v=vec2 {0, 0},
           vec2 _a=vec2 {0, 0});
    const char* name;
    virtual void tick(Uint64 dt);
    Texture texture;
    std::string text;
    SDL_Color textColor {0, 0, 0, 0};
  private:
    SDL_Renderer* renderer;
};


// объект "фон", отрисовывается два экземпляра 
// друг за другом по горизонтали, едет равномерно 
class Background : public Entity {
  public:
    using Entity::Entity;
    void tick(Uint64 dt);
};