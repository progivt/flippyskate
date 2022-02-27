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

Scene::Scene(int _W, int _H) : W{_W}, H{_H} {
    entities.push_back(&bg);
}

void Scene::update(Uint64 dt) {   
    for (auto& e : entities) {
        e->tick(dt);
    }
}

Level::Level(int _W, int _H) : Scene {_W, _H} {
    bg =        Background("bg2", 0,0,   -0.15,0);
    player =    Entity("skater", 100,100, 0,0.05, 0,GRAVITY);
    scorecard = Entity("*", W,20,    0,0);
    col1 =      Entity("col", 400,-H/3, -0.3,0);
    col2 =      Entity("col", 400+COLUMN_DIST,-50, -0.3,0);
    entities = std::vector<Entity *> {&bg, &player, &scorecard, &col1, &col2};

    scorecard.textColor = SDL_Color {0, 0, 33, 0};

    SDL_Log("Main level init ok");
}

void Level::reset() {

}


WelcomeScreen::WelcomeScreen (int _W, int _H) : Scene {_W, _H} {
    bg = Background("bg1", 0,0, 0,0, 0,0);
    start = Entity ("*", W/2,H/2);
    start.text = "START!";
    entities.push_back(&bg);
    entities.push_back(&start);
    SDL_Log("Welcome screen init ok");
}

void WelcomeScreen::update(Uint64 dt){
}

void WelcomeScreen::reset(){
}

void WelcomeScreen::handleEvent(SDL_Event e){
}

void Level::update(Uint64 dt){
    Scene::update(dt);

    if (player.py > H - player.texture.h) {
        player.py = H - player.texture.h - 1;
        SDL_Log("Dead!");
        player.name = "skater4";
        SDL_DestroyTexture(player.texture.sdlTexture);
        player.texture.sdlTexture = nullptr;
        player.vy = 0;
        player.ay = 0;

    }
     SDL_Log("py=%f", player.py);

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

