#include <cstdlib>
#include <cmath>

#include "scene.hpp"

#define SCROLL_SPEED (-0.19f)
#define GRAVITY      0.0007f
#define JUMP_SPEED   (-0.4f)
#define COLUMN_DIST  320
#define COLUMN_WIDTH 96

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


////////////////////////////////////////////////////////////////////////////////
Level::Level(int _W, int _H) : Scene {_W, _H} {
    bg =        Entity("bg2");
    player =    Entity("skater");
    entities = std::vector<Entity *> {&bg, &player};
    colIndex = entities.size();
    numCols = ceil(W / (float)COLUMN_DIST);
    for (int i=0; i<numCols; i++){
        Entity* column = new Entity("col");
        entities.push_back(column);
    }
    scorecard = Entity(TXTMARK);
    entities.push_back(&scorecard);
    reset();
    SDL_Log("Main level init ok");
}

void Level::reset() {
    score = 0;
    
    bg.pos = {0,0};   bg.v = {SCROLL_SPEED,0};
    
    player.pos = {100,100}; 
    player.v = {0,0}; 
    player.a = {0,0} ;
    
    scorecard.textColor = SDL_Color {255, 255, 255, 0};
    scorecard.text = "0";
    scorecard.pos = {(float)W, 20};
    
    for (int i=0; i<numCols; i++){
        entities[colIndex+i]->pos = { (float)W + i * COLUMN_DIST, -20 - (float) (300.0f*rand())/RAND_MAX};
        SDL_Log("Column created X=%f, Y=%f", entities[colIndex+i]->pos.x, entities[colIndex+i]->pos.y);
    }
    state = INTRO;
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

    for (int i = colIndex; i < colIndex + numCols; i++) {
        int prev = colIndex + (numCols + i - colIndex - 1) % numCols;
        if (entities[i]->pos.x < -COLUMN_WIDTH) {
            entities[i]->pos.x = entities[prev]->pos.x + COLUMN_DIST;
            entities[i]->pos.y = clamp(entities[prev]->pos.y  + 70 - (140.*rand()/RAND_MAX), -320, 0);
        }
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
            // Поехали!
            player.v.y = -0.05;
            player.a.y = GRAVITY;
            for (int i = colIndex; i < colIndex+numCols; i++){
                entities[i]->v.x = SCROLL_SPEED;
            }
            state = PLAYING;
            break;
        }
}
