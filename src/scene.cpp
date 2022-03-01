#include <cstdlib>
#include <cmath>

#include "scene.hpp"

#define SCROLL_SPEED (-0.29f)
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
    bg = Entity("bg2");
    player = Entity("skater"); 
    entities = std::vector<Entity *> {&bg, &player};
    colIndex = entities.size();
    numCols = ceil(W / (float)COLUMN_DIST);
    for (int i=0; i<numCols; i++){
        Entity* column = new Entity("col");
        entities.push_back(column);
    }
    scorecard = Entity(TXTMARK);
    entities.push_back(&scorecard);
    gameover = Entity("gameover");
    entities.push_back(&gameover);
    // highscore = Entity("highscore");

    reset();
    postEvent(TEXTURE_RELOAD, &scorecard);
    SDL_Log("Main level init ok");
}

void Level::reset() {
    score = 0;
    postEvent(TEXTURE_RELOAD, &scorecard);
    
    bg.pos = {0,0};   bg.v = {SCROLL_SPEED,0};
    
    player.name = "skater";
    player.pos = {100,100}; 
    player.v = {0,0}; 
    player.a = {0,0} ;
    
    scorecard.textColor = SDL_Color {255, 255, 255, 0};
    scorecard.text = "0";
    scorecard.pos = {(float)W, 20};

    gameover.pos = {W+.0f, H+.0f};
    gameover.visible = false;
    
    for (int i=0; i<numCols; i++){
        entities[colIndex+i]->pos = { (float)W + i * COLUMN_DIST, -20 - (float) (300.0f*rand())/RAND_MAX};
        SDL_Log("Column placed X=%f, Y=%f", entities[colIndex+i]->pos.x, entities[colIndex+i]->pos.y);
    }
    nextColumn = 0;

    state = INTRO;
}

void Level::update(Uint64 dt){
    Scene::update(dt);

    switch (state) {
      case PLAYING:
        if (player.pos.y < miny || player.pos.y > maxy) {
            startDeath();
            state = DYING;
            SDL_Log("DYING");
        } else {
            // прошли ли очередную колонну?
            if (player.pos.x > entities[colIndex+nextColumn]->pos.x + COLUMN_WIDTH / 2) 
            {
                score++;
                SDL_Log("Score: %d", score);
                scorecard.text = std::to_string(score);
                nextColumn = (nextColumn+1) % numCols;
                postEvent(TEXTURE_RELOAD, &scorecard);
            }
            // уехавшие за левый борт колонны респаунятся справа
            for (int i = 0; i < numCols; i++) {
                if (entities[colIndex+i]->pos.x < -COLUMN_WIDTH) {
                    respawnColumn(i);
                }
            }
        }
        break;
      case DYING:
        // останавливаем пике персонажа и надписей, если долетели
        gameover.pos.y = clamp(gameover.pos.y, 0, H/3);
        if (gameover.pos.y == H/3)
            gameover.v.y = gameover.a.y = 0;
        
        player.pos.y = clamp(player.pos.y, 0, maxy);
        if (player.pos.y == maxy) 
            player.v.y = player.a.y = 0;

        if (gameover.pos.y == H/3 && player.pos.y == maxy) {
        //     // доска очков выезжает
        //     highscore.pos.x = (W - highscore) /2;
        //     gameover.pos.y = -gameover.srcRect.h;
        //     gameover.a.y = 2*GRAVITY;
        //     entities.push_back(&gameover);
        // }
            state = DEAD;
            SDL_Log("DEAD");
        }
        break;
    }
}

void Level::handleEvent(SDL_Event event) {
    if (event.type == SDL_MOUSEBUTTONUP || 
        event.type == SDL_KEYUP && (
            event.key.keysym.sym == SDLK_SPACE || 
            event.key.keysym.sym == SDLK_UP    || 
            event.key.keysym.sym == SDLK_w)
        ) {
        switch (state){
          case PLAYING: 
            // прыжок!
            player.v.y = JUMP_SPEED;
            break;
          case INTRO:
            // Поехали!
            maxy = H - player.srcRect.h - 2; 
            miny = 2;
            scorecard.pos.x = W - scorecard.srcRect.w - 20;
            player.v.y = -0.05;
            player.a.y = GRAVITY;
            for (int i = colIndex; i < colIndex+numCols; i++){
                entities[i]->v.x = SCROLL_SPEED;
            }
            state = PLAYING;
            SDL_Log("PLAYING");
            break;
          case DEAD:
            reset();
            postEvent(TEXTURE_RELOAD, &player);
            break;
        }
    }
}

void Level::startDeath() {
    player.pos.y = clamp(player.pos.y, miny, maxy);
    for (auto& e: entities) {
        e->v.x = e->v.y = e->a.y = 0;
    }
    // поменять картинку
    player.name = "skater_dead";
    postEvent(TEXTURE_RELOAD, &player);
    
    // если в потолок, падаем
    if (player.pos.y < maxy) {
        player.a.y = 2*GRAVITY;
    };
    
    // гамовер выезжает
    gameover.pos.x = (W - 390) /2;
    gameover.pos.y = -gameover.srcRect.h;
    gameover.a.y = 2*GRAVITY;
    gameover.visible = true;
}

void Level::respawnColumn(int i){
    // респауним колонну справа, соблюдая интервал от крайней колонны
    int j = (numCols + i - 1) % numCols;
    vec2* curPos = &entities[colIndex+i]->pos;
    vec2* prevPos = &entities[colIndex+j]->pos;
    curPos->x = prevPos->x + COLUMN_DIST;
    curPos->y = clamp(prevPos->y  + 70 - (140.*rand()/RAND_MAX), -320, 0);
}