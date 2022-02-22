#include <iostream>

#include "game.h"

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char* argv[]) {

    Game game(WIDTH, HEIGHT, "Flappy Skater");

    SDL_Renderer *renderer = game.renderer;
    SDL_Texture *bg = IMG_LoadTexture(renderer, "./res/bg2.png");

    SDL_Event event;
    event.type = SDL_FIRSTEVENT;

    bool newPress = true;
    int i=0;
    Uint64 t0 = SDL_GetTicks64();
    Uint64 tLastRender = t0, t, dt;


    SDL_Rect bgDst {0, 0, 2*WIDTH, HEIGHT},
             bgSrc {0, 0, WIDTH, HEIGHT};
    float bgDstx = bgDst.x;
    int iters = 0;

    while (event.type != SDL_QUIT) {
        t = SDL_GetTicks64();
        dt = t - t0; 
        bgDstx -= 0.3 * dt;
        bgDst.x = bgDstx;
        if (bgDstx < -WIDTH) bgDstx = 0;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                float ms = SDL_GetTicks();
                std::cout << iters << " steps @" << ms << "ms, @" << 1000.0*iters/ms << " fps\n";
                break;
            }
            
            if (event.type == SDL_MOUSEMOTION) {
                std::cout << "mouse has been moved\n";
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && newPress) {
                    std::cout << "space was pressed\n";
                    newPress = false;
            }
            if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
                    newPress = true;
            }
        }

        if (t - tLastRender > 16) {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, bg, &bgSrc, &bgDst);
            SDL_RenderPresent(renderer);
            tLastRender = t;
            iters++;
        }
        t0 = t;
    }
    return 0;
}
