#include "startscreen.hpp"

StartScreen::StartScreen (Engine* _engine) : Scene {_engine, {"bg1", "*START!"}} {
    reset();
    SDL_Log("Start screen init ok");
}

void StartScreen::update(Uint64 dt){}

void StartScreen::reset(){
    Entity* label = entities[1];
    loadTextures();
    label->pos = {(W - label->srcRect.w) / 2.f,
                  (H - label->srcRect.h) / 2.f};
}

void StartScreen::handleEvent(SDL_Event e){}


