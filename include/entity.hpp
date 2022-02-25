// объект с текстурой, с вещественными 
// положением, скоростью и ускорением
#pragma once
#include <iostream>
#include <SDL2/SDL.h>

// текстура из файла и ее размеры
struct Texture {
    int w, h;
    SDL_Texture *sdlTexture;
    const char *imgFileName;
};

class Entity {
  public:
    float px, py, vx, vy, ax, ay;
    SDL_Rect srcRect;
    Entity(const char *_imgFileName=nullptr,
           float _px=0, float _py=0, 
           float _vx=0, float _vy=0, 
           float _ax=0, float _ay=0);
    virtual void tick(Uint64 dt);
    Texture texture;
  private:
    SDL_Renderer *renderer;
};

class Background : public Entity {
  public:
    using Entity::Entity;
    void tick(Uint64 dt);
};