#include <cstdlib>

#include "scene.hpp"

#define SCROLL_SPEED 0.0003f
#define GRAVITY 0.0007f
#define JUMP_SPEED -0.4f

int clamp(int x, int max, int min) {
    x = x>max ? max : x;
    return x<min ? min : x;
}

Scene::Scene(int _H, int _W) : W{_W}, H{_H} {
    SDL_Log("Scene init start");
    
    bg =     Background("bg2", 0,0, -0.15,0, 0,0);
    player = Entity("skater2", 100,0, 0,0.05, 0,GRAVITY);
    col1 =   Entity("col", 400,-H/3, -0.3,0, 0,0);
    col2 =   Entity("col", 700,0, -0.3,0, 0,0);
    entities = std::vector<Entity *> {&bg, &player, &col1, &col2};

    SDL_Log("Scene init ok");
}

void Scene::update(Uint64 dt) {    
    for (auto& e : entities) {
        e->tick(dt);
    }
    if (col1.px < -col1.srcRect.w) {
        col1.px = W;
        col1.py = col2.px + (1.*rand())/RAND_MAX * 100;
    } 
    col2.px = col2.px < -col2.srcRect.w ? W : col2.px;
}

void Scene::handleEvent(SDL_Event event) {
    switch (event.type) {
      case SDL_MOUSEBUTTONUP:
        player.vy = JUMP_SPEED;
        break;
      case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_SPACE || 
            event.key.keysym.sym == SDLK_UP || 
            event.key.keysym.sym == SDLK_w || 
            event.key.keysym.sym == SDLK_w)
            player.vy = JUMP_SPEED;
    }
}
