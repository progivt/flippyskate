// объект с с вещественными  положением, скоростью и ускорением, плюс
// и инфо о текстуре а) из файла name.png либо 
//                   б) отрисовкой текста из поля text цвета textColor
#pragma once
#include <iostream>
#include <SDL2/SDL.h>

#include "util.hpp"
#define TXTMARK "*" // name[0]==TXTMARK, означает что это текст

class Entity {
  public:
    const char* name;
    vec2 pos, v, a;
    bool visible;
    SDL_Rect srcRect;
    Texture* texture;
    std::string text;
    SDL_Color textColor {0, 0, 0, 0};
    Entity(const char *_imgFileName=nullptr,
           vec2 _pos=vec2 {0, 0},
           vec2 _v=vec2 {0, 0},
           vec2 _a=vec2 {0, 0});
    virtual void tick(Uint64 dt);
    
};
