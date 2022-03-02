#include "startscreen.hpp"

StartScreen::StartScreen (Engine* _engine) 
  : 
  Scene {_engine, {"bg1", "skaterbtn", "scoresbtn"}} {
    reset();
    // SDL_Log("Start screen init ok");
}

void StartScreen::update(Uint64 dt){}

void StartScreen::reset(){
    loadTextures();
    int btnWidth = entities[1]->srcRect.w;
    for (int i=0; i<2; i++){
        entities[1+i]->pos = {W/2.f - btnWidth - 10 + i*(btnWidth + 20), 0.4f*H};
        btnRect[i] = entities[1+i]->srcRect;
        btnRect[i].x = entities[1+i]->pos.x;
        btnRect[i].y = entities[1+i]->pos.y;
    }                       
}

void StartScreen::handleEvent(SDL_Event e){
    SDL_Point pos;
    if (e.type == SDL_MOUSEBUTTONUP)
        pos = {((SDL_MouseButtonEvent*)&e)->x, ((SDL_MouseButtonEvent*)&e)->y};
    if (e.type == SDL_MOUSEBUTTONUP && SDL_PointInRect(&pos, &btnRect[0])|| 
        e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_SPACE)
        postEvent(GO_PLAY); 
    if (e.type == SDL_MOUSEBUTTONUP && SDL_PointInRect(&pos, &btnRect[1]))
        postEvent(GO_SCORES); 
}
