#include "gamescreen.hpp"

#define SCROLL_SPEED (-0.29f)
#define GRAVITY      0.0008f
#define JUMP_SPEED   (-0.3f)
#define COLUMN_DIST  320
#define COLUMN_WIDTH 96

GameScreen::GameScreen(Engine* _engine) 
  : Scene {_engine, {"bg2"}} {
    bg = entities[0];

    // колонны
    col0 = entities.size();
    nCols = ceil(W / (float)COLUMN_DIST);
    for (int i = 0; i < nCols; i++){
        entities.push_back(new Entity{"col"});
    }
    
    createEntities({"player", TXTMARK});
    player    = entities[col0+nCols]; 
    scorecard = entities[col0+nCols+1];
    
    ovrl0 = entities.size(); 
    createEntities({"gameover", "finalbox", TXTMARK, TXTMARK, "medal"});
    gameover  = entities[ovrl0]; 
    finalbox  = entities[ovrl0+1];
    yourscore = entities[ovrl0+2];
    highscore = entities[ovrl0+3];
    medal     = entities[ovrl0+4];
    
    reset();
    SDL_Log("Main level init ok");
}

void GameScreen::reset() {
    displayOverlays(false);
    engine->setFontSize();

    score = 0;
    scorecard->text = "0"; 
    yourscore->text="YOUR SCORE: "; 
    highscore->text = "HISCORE: ";
    
    player->name = "player";
    loadTextures();

    bg->pos = {0,0};   
    bg->v = {SCROLL_SPEED,0};
    player->pos = {100, .y = H/2.f - player->srcRect.h};    
    player->v = player->a = {0,0}; 
    scorecard->pos = {(float)W - scorecard->srcRect.w - 20, 20};
    for (int i=0; i<nCols; i++){
        entities[col0+i]->pos = { (float)W + i * COLUMN_DIST, 
            -20 - (300.0f * rand())/RAND_MAX};
        SDL_Log("Column placed X=%f, Y=%f", 
                entities[col0+i]->pos.x, entities[col0+i]->pos.y);
    }
    maxy = H - player->srcRect.h - 2; 
    miny = 2;
    nextColumn = 0;
    state = INTRO;
}

void GameScreen::update(Uint64 dt){
    Scene::update(dt);

    switch (state) {
      case PLAYING:
        if (player->pos.y < miny || player->pos.y > maxy || isColliding()) {
            startDeath();
            state = DYING;
            postEvent(UPD_SCORES, &score);
            if (score > maxScore){
                maxScore = score;
                SDL_Log("Set hiscore to %d", maxScore);
            }
            SDL_Log("DYING");
        } else {
            // прошли ли очередную колонну?
            if (player->pos.x > entities[col0+nextColumn]->pos.x + COLUMN_WIDTH / 2) 
            {
                score++;
                SDL_Log("Score: %d", score);
                scorecard->text = std::to_string(score);
                nextColumn = (nextColumn+1) % nCols;
                engine->loadEntityTexture(scorecard);
                scorecard->pos.x = W - scorecard->srcRect.w - 20;
            }
            // уехавшие за левый борт колонны респаунятся справа
            for (int i = 0; i < nCols; i++) {
                if (entities[col0+i]->pos.x < -COLUMN_WIDTH) {
                    respawnColumn(i);
                }
            }
        }
        break;
      case DYING:
        // останавливаем пике персонажа и надписей, если долетели
        gameover->pos.y = clamp(gameover->pos.y, 0, H/5);
        if (gameover->pos.y == H/5)
            gameover->v = gameover->a = {0,0};
        
        player->pos.y = clamp(player->pos.y, 0, maxy);
        if (player->pos.y == maxy) 
            player->v = player->a = {0,0};

        if (gameover->pos.y == H/5 && player->pos.y == maxy) {
            // запускаем вылет доски 
            for (int i=ovrl0 + 1; i<entities.size(); i++) {
                entities[i]->a = {0, -8* GRAVITY};
            }
        }
        if (gameover->pos.y == H/5 && player->pos.y == maxy && finalbox->pos.y <= 0.4*H){
            // останавливаем доску
            finalbox->pos.y = 0.4*H;
            for (int i=ovrl0; i<entities.size(); i++) {
                entities[i]->v = entities[i]->a = {0, 0};
            }
            yourscore->pos = finalbox->pos + vec2{145, 65};
            highscore->pos = finalbox->pos + vec2{145, 110};
            medal->pos = finalbox->pos + vec2{40, 61};
            state = DEAD;
            SDL_Log("DEAD");
        }
        break;
    }
}

void GameScreen::handleEvent(SDL_Event event) {
    if (event.type == SDL_MOUSEBUTTONUP || 
        event.type == SDL_KEYUP && (
            event.key.keysym.sym == SDLK_SPACE || 
            event.key.keysym.sym == SDLK_UP    || 
            event.key.keysym.sym == SDLK_w)
        ) {
        switch (state){
          case PLAYING: 
            // прыжок!
            player->v.y = JUMP_SPEED;
            break;
          case INTRO:
            // Поехали!
            player->v.y = -0.05;
            player->a.y = GRAVITY;
            for (int i = col0; i < col0+nCols; i++){
                entities[i]->v.x = SCROLL_SPEED;
            }
            state = PLAYING;
            SDL_Log("PLAYING");
            break;
          case DEAD:
            reset();
            break;
        }
    }
    if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE && state == DEAD)
        postEvent(GO_START);
}

bool GameScreen::isColliding(){
    // рассматриваем след колонну и +одну за ней 
    for (int i=0; i<2; i++){
        // костылизм
        Entity* col = entities[col0 + (nextColumn + i)%nCols];
        float px = player->pos.x,     py = player->pos.y,
              cx = col->pos.x,        cy = col->pos.y;
        int   pw = player->srcRect.w, ph = player->srcRect.h;
        if (px+pw > cx && px < cx + COLUMN_WIDTH && py + 7  < cy + 425 || 
            px+pw > cx && px < cx + COLUMN_WIDTH && py + ph > cy + 650)
            return true; 
    }
    return false;
}

void GameScreen::startDeath() {
    for (auto& e: entities) {
        e->v.x = e->v.y = e->a.y = 0;
    }
    // поменять картинку
    player->pos.y = clamp(player->pos.y, miny, maxy);
    player->name = "player_lose";
    engine->loadEntityTexture(player);
    
    // если в потолок или колонну, падаем
    if (player->pos.y < maxy) {
        player->a.y = 2*GRAVITY;
    };

    overlayReset();
    medal->visible = (score > 0 && score >= maxScore);
}

void GameScreen::displayOverlays(bool show){
    for (int i=ovrl0; i<entities.size(); i++)
        entities[i]->visible = show;
}

// расположить выезжающие элементы проигрыша
void GameScreen::overlayReset(){
    SDL_Log("Hiscore is %d", maxScore);
    gameover->pos = {(W - gameover->srcRect.w)/2.f, (float)-gameover->srcRect.h};
    gameover->v = {0,0}; 
    gameover->a.y = 2*GRAVITY;
    
    finalbox->pos = {(W - finalbox->srcRect.w)/2.f, H+0.f};
    finalbox->v = finalbox->a = {0,0}; 

    engine->setFontSize(28);
    
    yourscore->text += std::to_string(score);
    yourscore->pos = finalbox->pos + vec2{145, 65};
    yourscore->v = yourscore->a = {0,0}; 
    engine->loadEntityTexture(yourscore);

    highscore->text += std::to_string(maxScore);
    highscore->pos = finalbox->pos + vec2{145, 110};
    highscore->v = yourscore->a = {0,0}; 
    engine->loadEntityTexture(highscore);

    medal->pos = finalbox->pos + vec2{40, 61};
    medal->v = medal->a = {0,0}; 

    displayOverlays(true);
}

void GameScreen::respawnColumn(int i){
    // респауним колонну справа, соблюдая интервал от крайней колонны
    int j = (nCols + i - 1) % nCols;
    vec2* curPos = &entities[col0+i]->pos;
    vec2* prevPos = &entities[col0+j]->pos;
    curPos->x = prevPos->x + COLUMN_DIST;
    curPos->y = clamp(prevPos->y  + 70 - (140.*rand()/RAND_MAX), -320, 0);
}