#include <cstdlib>

#include "scene.hpp"

#define SCROLL_SPEED (0.0003f)
#define GRAVITY      (0.0007f)
#define JUMP_SPEED   (-0.4f)
#define COLUMN_DIST     (420)

int clamp(int x, int min, int max) {
    x = x > max ? max : x;
    return x < min ? min : x;
}

Scene::Scene(int _H, int _W) : W{_W}, H{_H} {
    entities.push_back(&bg);
}

void Scene::update(Uint64 dt) {   
    for (auto& e : entities) {
        e->tick(dt);
    }
}

void Scene::handleEvent(SDL_Event event) {
}

Level::Level(int _W, int _H) : Scene {_W, _H} {
    SDL_Log("Main level init start");
    
    bg =        Background("bg2", 0,0, -0.15,0, 0,0);
    player =    Entity("skater2", 100,0, 0,0.05, 0,GRAVITY);
    scorecard = Entity("*", W,20, 0, 0,0,0);
    col1 =      Entity("col", 400,-H/3, -0.3,0, 0,0);
    col2 =      Entity("col", 400+COLUMN_DIST,-50, -0.3,0, 0,0);
    entities = std::vector<Entity *> {&bg, &player, &scorecard, &col1, &col2};

    scorecard.textColor = SDL_Color {0, 0, 33, 0};

    SDL_Log("Main level init ok");
}

void Level::update(Uint64 dt){
    Scene::update(dt);
    if (col1.px < -col1.srcRect.w) {
        col1.px = col2.px + COLUMN_DIST;
        col1.py = clamp(col2.py  + 70 - (140.*rand()/RAND_MAX), -320, 0);
    } 

    if (col2.px < -col2.srcRect.w) {
        col2.px = col1.px + COLUMN_DIST;
        col2.py = clamp(col1.py  + 70 - (140.*rand()/RAND_MAX), -320, 0);
    } 
}

void Level::handleEvent(SDL_Event event) {
    switch (event.type) {
      case SDL_MOUSEBUTTONUP:
        player.vy = JUMP_SPEED;
        break;
      case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_SPACE || 
            event.key.keysym.sym == SDLK_UP    || 
            event.key.keysym.sym == SDLK_w     || 
            event.key.keysym.sym == SDLK_w)
            player.vy = JUMP_SPEED;
    }
}