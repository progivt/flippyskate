// экран рекордов
#pragma once
#include "scoresscreen.hpp"

class ScoresScreen : public Scene {
  public:
    ScoresScreen(Engine* _engine);
    Entity* header;
    std::vector<unsigned> scores {0U,0U,0U};
    void reset();
    void handleEvent(SDL_Event e);
  private:
    void renderScores();
};
