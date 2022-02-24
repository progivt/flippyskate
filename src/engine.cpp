#include <iostream>
#include <memory>

#include "engine.hpp"

#define RENDER_THRESHOLD_MS 16

void die(const char* msg1, const char* msg2, int errorCode=1) {
    std::cerr << msg1 << msg2;
    exit(errorCode);
}

void Engine::initSDL(int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO)<0) 
        die("SDL init error: ", SDL_GetError());    
    
    if (IMG_Init(IMG_INIT_PNG) ^ IMG_INIT_PNG) 
        die("SDL_image init error: ", IMG_GetError());
    
    window = SDL_CreateWindow("Flappy Skater",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              width, height,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Engine::Engine(int width, int height) 
    : W {width}, H {height} {
    initSDL(width, height);

    loadSprite("./res/bg2.png");
    loadSprite("./res/skater2.png");
    loadSprite("./res/podium.png");
    
    bg =     Background(renderer, &sprites[0], 0,0, -0.3,0, 0,0);
    player = Entity(renderer, &sprites[1], 100,0, 0,0.05, 0,0.0002);
    col1 =   Entity(renderer, &sprites[2], W-100,H/2, -0.3,0, 0,0);
    col2 =   Entity(renderer, &sprites[2], W-300,2*H/3, -0.3,0, 0,0);
    
    entities = std::vector<Entity *> {&bg, &player, &col1, &col2};

    lastDrawTime = lastTime = SDL_GetTicks64();
    ticks = frames = 0;
    exiting = false;
    repaint();
}

void Engine::loadSprite(const char* path) { 
    sprites.push_back(Sprite(renderer, path));
}

void Engine::update() {
    Uint64 t = SDL_GetTicks64(), 
           dt = t - lastTime; 
    
    for (auto& e : entities) {
        e->tick(dt);
    }

    if (bg.px < -W) 
        bg.px = 0;

    col1.px = col1.px < -col1.srcRect.w ? W : col1.px;

    lastTime = t;
    ticks++;

    if (t - lastDrawTime >= RENDER_THRESHOLD_MS)
        repaint();
}

void Engine::handleEvent(SDL_Event event) {
    switch (event.type) {
      case SDL_QUIT:
        std::cout << ticks << " ticks and "
                  << frames << " frames in " 
                  << lastTime << "ms, @" 
                  << 1000.0*ticks/lastTime << " ticks per sec\n" 
                  << 1000.0*frames/lastTime << " fps\n";
        this->exiting = true;
        break;
      case SDL_MOUSEBUTTONUP:
        player.vy = -JUMP_ACCEL;
        break;
      case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_SPACE)
            player.vy = -JUMP_ACCEL;
    }
}

void Engine::repaint() {
    SDL_RenderClear(renderer);
    for (auto& e : entities) {
        e->draw();
    }
    SDL_RenderPresent(renderer);
    lastDrawTime = SDL_GetTicks64();
    frames++;
}

Engine::~Engine() {
    for (auto& s : sprites) {
        SDL_DestroyTexture(s.texture);
    }
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_Delay(500);
    SDL_Quit();
}
