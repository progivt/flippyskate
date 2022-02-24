#include "engine.hpp"

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char* argv[]) {
    Engine game(WIDTH, HEIGHT);

    while (!game.exiting) {
        game.update();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            game.handleEvent(event);
        }
    }
    return 0;
}
