// объект с текстурой либо отрисованным текстом, 
// с вещественными  положением, скоростью и ускорением
#pragma once
#include <iostream>
#include <SDL2/SDL.h>

#define TXTMARK '*'

struct vec2 {
    float x, y;
    vec2& operator+=(const vec2& v){ x += v.x; y += v.y; return *this;}
    vec2& operator*=(const float a){ x *= a; y *= a; return *this;}
    friend vec2 operator+(vec2 lhs, const vec2& rhs) { return lhs += rhs; }
    friend vec2 operator*(const float a, vec2 v) { v *= a; return v; }
};


// текстура из файла и ее размеры
struct Texture {
    int w, h;
    SDL_Texture *sdlTexture;
};



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

class Background : public Entity {
  public:
    using Entity::Entity;
    void tick(Uint64 dt);
};