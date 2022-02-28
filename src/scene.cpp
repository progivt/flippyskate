#include <cstdlib>

#include "scene.hpp"

#define SCROLL_SPEED (-0.15f)
#define GRAVITY      (0.0007f)
#define JUMP_SPEED   (-0.4f)
#define COLUMN_DIST     (420)

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

Level::Level(int _W, int _H) : Scene {_W, _H} {
    bg =        Entity("bg2");
    player =    Entity("skater");
    scorecard = Entity("*");
    col1 =      Entity("col", {(float)W,0});
    col2 =      Entity("col", {(float)W,0});
    entities = std::vector<Entity *> {&bg, &player, &scorecard, &col1, &col2};
    scorecard.textColor = SDL_Color {255, 255, 255, 0};
    reset();
    SDL_Log("Main level init ok");
}

void Level::reset() {
    score = 0;
    bg.pos = {0,0};   bg.v = {SCROLL_SPEED,0};
    player.pos = {100,100}; player.v = {0,0}; player.a = {0,0} ;
    scorecard.text = "0";
    scorecard.pos = {(float)W,20};
    entities = std::vector<Entity *> {&bg, &player, &scorecard};
    state = INTRO;
}


WelcomeScreen::WelcomeScreen (int _W, int _H) : Scene {_W, _H} {
    bg = Entity("bg1", {0,0}, {0,0}, {0,0});
    start = Entity ("*", {W/2.f,H/2.f});
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

    maxy = H - player.srcRect.h - 2;
    miny = 2;

    if (player.pos.y < miny || player.pos.y > maxy) {
        player.pos.y = clamp(player.pos.y, miny, maxy);
        for (auto& e: entities) {
            e->v.x = e->v.y = e->a.y = 0;
        }
        state = DEAD;
        SDL_DestroyTexture(player.texture.sdlTexture);
        player.texture.sdlTexture = nullptr;
        player.name = "skater_dead";
    }

    if (col1.pos.x < -col1.srcRect.w) {
        col1.pos.x = col2.pos.x + COLUMN_DIST;
        col1.pos.y = clamp(col2.pos.y  + 70 - (140.*rand()/RAND_MAX), -320, 0);
    } 

    if (col2.pos.x < -col2.srcRect.w) {
        col2.pos.x = col1.pos.x + COLUMN_DIST;
        col2.pos.y = clamp(col1.pos.y  + 70 - (140.*rand()/RAND_MAX), -320, 0);
    } 
}

void Level::handleEvent(SDL_Event event) {
    if (event.type == SDL_MOUSEBUTTONUP || 
        event.type == SDL_KEYUP && (
            event.key.keysym.sym == SDLK_SPACE || 
            event.key.keysym.sym == SDLK_UP    || 
            event.key.keysym.sym == SDLK_w)
        ) 
        switch (state){
          case PLAYING: 
            player.v.y = JUMP_SPEED;
            break;
          case INTRO:
            player.v.y = -0.05;
            player.a.y = GRAVITY;
            col1.pos = {(float)W, (float)-H/3};
            col2.pos = {(float)W + COLUMN_DIST, (float)-H/5};
            col1.v = col2.v = {SCROLL_SPEED,0};
            entities = std::vector<Entity *> {&bg, &player, &scorecard, &col1, &col2};
            state = PLAYING;
            break;
        }
}
