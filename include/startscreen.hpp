#pragma once
#include "scene.hpp"

// начальный экран
class StartScreen : public Scene {
  public:
    StartScreen(Engine* _engine);
    Entity start;
    void reset();
    void update(Uint64 dt);
    void handleEvent(SDL_Event e);     
};
