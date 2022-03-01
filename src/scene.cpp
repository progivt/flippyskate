#include <cstdlib>
#include <cmath>

#include "scene.hpp"

Scene::Scene(int _W, int _H) : W{_W}, H{_H} {
    entities.push_back(&bg);
}

void Scene::update(Uint64 dt) {   
    for (auto& e : entities){
        e->tick(dt);
        if (e == &bg && e->pos.x < -W) 
            e->pos.x = 0;
    }
}


////////////////////////////////////////////////////////////////////////////////
WelcomeScreen::WelcomeScreen (int _W, int _H) : Scene {_W, _H} {
    bg = Entity("bg1", {0,0}, {0,0}, {0,0});
    start = Entity (TXTMARK, {W/2.f,H/2.f});
    start.text = "START!";
    entities.push_back(&bg);
    entities.push_back(&start);
    SDL_Log("Welcome screen init ok");
}

void WelcomeScreen::update(Uint64 dt){}

void WelcomeScreen::reset(){}

void WelcomeScreen::handleEvent(SDL_Event e){}


