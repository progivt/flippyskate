#include "startscreen.hpp"

StartScreen::StartScreen (Engine* _engine) : Scene {_engine, {"bg1"}} {
    start = Entity (TXTMARK, {W/2.f,H/2.f});
    start.text = "START!";
    entities.push_back(&start);
    SDL_Log("Start screen init ok");
}

void StartScreen::update(Uint64 dt){}

void StartScreen::reset(){}

void StartScreen::handleEvent(SDL_Event e){}


