#include <iostream>

#include "game.hpp"

#define WIDTH 800
#define HEIGHT 600
#define JUMP_ACCEL 0.15f

int main(int argc, char* argv[]) {
    Game game(WIDTH, HEIGHT, "Flappy Skater");

    game.loadSprite("./res/bg2.png");
    game.loadSprite("./res/skater2.png");
    game.loadSprite("./res/podium.png");
    
    game.createEntity(game.sprites[0], 0,0, -0.3,0, 0,0);
    game.createEntity(game.sprites[0], WIDTH,0, -0.3,0, 0,0);
    game.createEntity(game.sprites[1], 100,0, 0,0.05, 0,0.0002);
    game.createEntity(game.sprites[2], WIDTH-100,HEIGHT/2, -0.3,0, 0,0);
    game.createEntity(game.sprites[2], WIDTH-300,2*HEIGHT/3, -0.3,0, 0,0);
    
    Entity *bg = &game.entities[0],
           *bg2 = &game.entities[1],
           *playerChar = &game.entities[2];

    int frames = 0, ticks = 0;
    bool exiting = false;
    
    game.draw();
    frames++;
    while (!exiting) {
        game.tick();
        ticks++;

        if (bg->px < -WIDTH) {
            bg->px = 0;
            bg2->px = WIDTH;
        }

        for (auto lColumn = game.entities.begin()+3; lColumn != game.entities.end(); lColumn++){
            if (lColumn->px < -lColumn->W)
                lColumn->px = WIDTH;
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type){
            case SDL_QUIT:
                std::cout << game.ticks << " ticks and "
                          << game.frames << " frames in " 
                          << game.lastTime << "ms, @" 
                          << 1000.0*game.ticks/game.lastTime << " ticks per sec\n" 
                          << 1000.0*game.frames/game.lastTime << " fps\n";
                exiting = true;
                break;
            case SDL_MOUSEBUTTONUP:
                playerChar->vy = -JUMP_ACCEL;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_SPACE) {
                    playerChar->vy = -JUMP_ACCEL;
                }
            }
        }
    }
    return 0;
}
