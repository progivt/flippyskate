#pragma once
#include "scene.hpp"

#define GO_PLAY   100

// начальный экран
class StartScreen : public Scene {
  public:
    StartScreen(Engine* _engine);
    Entity start;
    void reset();
    void update(Uint64 dt);
    void handleEvent(SDL_Event e);
};
