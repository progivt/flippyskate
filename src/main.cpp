#include <iostream>

#include "game.hpp"

#define WIDTH 800
#define HEIGHT 600
#define JUMP_ACCEL 0.15f
#define RENDER_THRESHOLD_MS 16

int main(int argc, char* argv[]) {
    Game game(WIDTH, HEIGHT, "Flappy Skater");
    game.LoadSprite("./res/bg2.png");
    game.LoadSprite("./res/skater.png");
    game.CreateEntity(game.sprites[0], 0,0, -0.3,0, 0,0);
    game.CreateEntity(game.sprites[0], WIDTH,0, -0.3,0, 0,0);
    game.CreateEntity(game.sprites[1], 100,0, 0,0.05, 0,0.0002);
    Entity *bg = &game.entities[0],
           *bg2 = &game.entities[1],
           *hero = &game.entities[2];

    Uint64 lastTime = SDL_GetTicks64(), lastDrawTime = lastTime, t, dt;
    int frames = 0, ticks = 0;
    bool exiting = false;
    while (!exiting) {
        t = SDL_GetTicks64();
        dt = t - lastTime; 
        game.Tick(dt);
        ticks++;

        if (bg->px < -WIDTH) {
            bg->px = 0;
            bg2->px = WIDTH;
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type){
            case SDL_QUIT:
                std::cout << ticks << " ticks in " << t << "ms, @" << 1000.0*ticks/t << " fps\n";
                std::cout << frames << " frames in " << t << "ms, @" << 1000.0*frames/t << " fps\n";
                exiting = true;
                break;
            case SDL_MOUSEBUTTONUP:
                hero->vy = -JUMP_ACCEL;
                break;
            case SDL_KEYUP:
                std::cout << "SDL_KEYUP" << "\n";
                if (event.key.keysym.sym == SDLK_SPACE) {
                    hero->vy = -JUMP_ACCEL;
                }
            }
        }

        if (t - lastDrawTime > RENDER_THRESHOLD_MS) {
            game.draw();
            frames++;
            lastDrawTime = t;
        }
        lastTime = t;
    }
    return 0;
}
