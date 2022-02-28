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
    bg =        Background("bg2");
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
    bg.pos = {0,0};   bg.v = {-0.15f,0};
    player.pos = {100,100}; player.v = {0,0}; player.a = {0,0} ;
    //player.v = {0,0.05}; player.a = {0,GRAVITY}
    scorecard.text = "0";
    scorecard.pos = {(float)W,20};
    //400,(float)-H/3}, {-0.3,0});
    //400+COLUMN_DIST,-50}, {-0.3,0});
    entities = std::vector<Entity *> {&bg, &player, &scorecard};
    state = INTRO;
}


WelcomeScreen::WelcomeScreen (int _W, int _H) : Scene {_W, _H} {
    bg = Background("bg1", {0,0}, {0,0}, {0,0});
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

    if (player.pos.y > H - player.texture.h) {
        player.pos.y = H - player.texture.h - 1;
        SDL_Log("Dead!");
        player.name = "skater4";
        SDL_DestroyTexture(player.texture.sdlTexture);
        player.texture.sdlTexture = nullptr;
        player.v.y = 0;
        player.a.y = 0;

    }
     SDL_Log("py=%f", player.pos.y);

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
        event.type == SDL_KEYUP && 
            (event.key.keysym.sym == SDLK_SPACE || 
            event.key.keysym.sym == SDLK_UP    || 
            event.key.keysym.sym == SDLK_w     || 
            event.key.keysym.sym == SDLK_w)
        ) 
        switch (state){
          case PLAYING: 
            player.v.y = JUMP_SPEED;
            break;
          case INTRO:
            player.v.y = -0.05;
            player.a.y = GRAVITY;
            state = PLAYING;
            break;
        }
}

