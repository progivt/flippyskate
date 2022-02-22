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
    game.CreateEntity(game.sprites[1], 100,0, 0,0.05, 0,0.0002);
    Entity *bg = &game.entities[0],
           *hero = &game.entities[1];

    SDL_Event event;
    event.type = SDL_FIRSTEVENT;

    bool newPress = true;
    Uint64 t0 = SDL_GetTicks64(), tLastRender = t0, t, dt;

    int drawiters = 0, iters = 0;
    
    while (event.type != SDL_QUIT) {
        iters++;
        t = SDL_GetTicks64();
        dt = t - t0; 
        bg->tick(dt);
        hero->tick(dt);

        if (bg->px < -WIDTH) bg->px = 0;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                float ms = SDL_GetTicks();
                std::cout << iters << " ticks @" << ms << "ms, @" << 1000.0*iters/ms << " fps\n";
                std::cout << drawiters << " frames @" << ms << "ms, @" << 1000.0*drawiters/ms << " fps\n";
                break;
            }
            
            if (event.type == SDL_MOUSEMOTION) {
                std::cout << "mouse has been moved\n";
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && newPress) {
                hero->vy = -0.1;
                newPress = false;
            }
            if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
                newPress = true;
            }
        }

        if (t - tLastRender > 16) {
            SDL_RenderClear(renderer);
            bg->draw();
            bg->draw(bg->px+WIDTH, bg->py);
            hero->draw();
            SDL_RenderPresent(renderer);
            tLastRender = t;
            drawiters++;
        }
        t0 = t;
    }
    return 0;
}
