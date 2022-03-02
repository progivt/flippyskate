// начальный экран
#pragma once
#include "scene.hpp"

class StartScreen : public Scene {
  public:
    StartScreen(Engine* _engine);
    SDL_Rect btnRect[2];
    void reset();
    void update(Uint64 dt);
    void handleEvent(SDL_Event e);
};
