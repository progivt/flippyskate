#include <iostream>

#include "game.hpp"

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char* argv[]) {

    Game game(WIDTH, HEIGHT, "Flappy Skater");

    SDL_Renderer *renderer = game.renderer;
    game.LoadSprite("./res/bg2.png");
    game.LoadSprite("./res/skater.png");
    game.CreateEntity(game.sprites[0], 0,0, -0.3,0, 0,0);
    game.CreateEntity(game.sprites[0], WIDTH,0, -0.3,0, 0,0);
    game.CreateEntity(game.sprites[1], 100,0, 0,0.05, 0,0.0002);
    Entity *bg = &game.entities[0],
           *bg2 = &game.entities[1],
           *hero = &game.entities[2];

    SDL_Event event;
    event.type = SDL_FIRSTEVENT;

    bool newPress = true;
    Uint64 lastTime = SDL_GetTicks64(), lastRenderTime = lastTime, t, dt;

    int frames = 0, ticks = 0;
    
    while (event.type != SDL_QUIT) {
        t = SDL_GetTicks64();
        dt = t - lastTime; 
        game.Tick(dt);
        ticks++;

        if (bg->px < -WIDTH) {
            bg->px = 0;
            bg2->px = WIDTH;
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                float ms = SDL_GetTicks();
                std::cout << ticks << " ticks @" << ms << "ms, @" << 1000.0*ticks/ms << " fps\n";
                std::cout << frames << " frames @" << ms << "ms, @" << 1000.0*frames/ms << " fps\n";
                break;
            }
            
            if (event.type == SDL_MOUSEBUTTONUP || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && newPress) {
                hero->vy = -0.15;
                newPress = false;
            }
            if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
                newPress = true;
            }
        }

        if (t - lastRenderTime > 16) {
            game.draw();
            lastRenderTime = t;
            frames++;
        }
        lastTime = t;
    }
    return 0;
}
