#pragma once
#include <SDL2/SDL.h>

#define GO_START    100
#define GO_PLAY     101
#define GO_SCORES   102
#define UPD_SCORES  200

// стандартный двумерный вектор 
struct vec2 {
    float x, y;
    vec2& operator+=(const vec2& v){ x += v.x; y += v.y; return *this; }
    vec2& operator*=(const float a){ x *= a; y *= a; return *this; } 
    friend vec2 operator+(vec2 lhs, const vec2& rhs){ return lhs += rhs; }
    friend vec2 operator*(const float a, vec2 v){ v *= a; return v; }
};

// текстура из файла и ее размеры
struct Texture {
    int w, h;
    SDL_Texture *sdlTexture;
};

int clamp(int x, int min, int max);

// посылка события SDL для общения Scene с Game
void postEvent(Sint32 code, void* d1=NULL, void* d2=NULL);
