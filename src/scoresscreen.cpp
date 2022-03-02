#include "scene.hpp"
#include "scoresscreen.hpp"

ScoresScreen::ScoresScreen (Engine* _engine) 
  : Scene {_engine, {"bg3", 
                     "*HISCORES", 
                     "*ALMA SCHEGLOVA . . . . . . . . . ", 
                     "*NATASHA KHRABROVA. . . . ",
                     "*SATORI NAKAMOTO. . . . . . . ",
                    }
          } 
{
    header = entities[1];
    reset();
    SDL_Log("Start screen init ok");
}

void ScoresScreen::reset(){
    engine->loadEntityTexture(entities[0]);
    engine->loadEntityTexture(header);
    header->pos = {(W - header->srcRect.w)/2.f, H/4.f};
    renderScores();
    for (int i=0; i<3; i++){
        entities[2+i]->pos = {W*.16f, H*0.38f + i*1.1f*entities[2+i]->srcRect.h};
    }
}

void ScoresScreen::renderScores(){
    engine->setFontSize(30);
    for (int i=0; i<3; i++){
        entities[2+i]->text = entities[2+i]->name + 1;
        entities[2+i]->text += std::to_string(scores[i]);
        entities[2+i]->textColor = SDL_Color {0, 0, 0, 0};  
        engine->loadEntityTexture(entities[2+i]);
    }
    engine->setFontSize();
}

void ScoresScreen::handleEvent(SDL_Event e){
    if (e.type == SDL_MOUSEBUTTONUP || 
        e.type == SDL_KEYUP){
        postEvent(GO_START);
    }
}
