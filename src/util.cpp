#include "util.hpp"

int clamp(int x, int min, int max) {
    x = x > max ? max : x;
    return x < min ? min : x;
}

void postEvent(Sint32 code,void* d1, void* d2) {
    SDL_Event e;
    e.type = SDL_USEREVENT;
    e.user.code = code;
    e.user.data1 = d1;
    e.user.data2 = d2;
    SDL_PushEvent(&e);
}