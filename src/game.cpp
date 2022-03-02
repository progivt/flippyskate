#include "game.hpp"

#define WIDTH  800
#define HEIGHT 600
#define RENDER_QUANTUM 16

Game::Game() 
  : engine { Engine(WIDTH, HEIGHT) },
    gameScreen { GameScreen(&engine) },
    startScreen { StartScreen (&engine) } {
        
    scenes = std::vector<Scene *> {&startScreen, &gameScreen};
    
    startScreen.start.textColor = SDL_Color {255,255,255,0};
    startScreen.start.pos.x = (WIDTH  - startScreen.start.srcRect.w) / 2;
    startScreen.start.pos.y = (HEIGHT - startScreen.start.srcRect.h) / 2;

    highScore = 0;
    gameScreen.scorecard->text = "0";
    currentScene = &gameScreen;
    
    repaint();
    ticks = 0; 
    frames = 1;
    lastDrawTime = lastTime = SDL_GetTicks64();
    SDL_Log("Game init ok");
}


void Game::run(){
    bool exiting = false;
    SDL_Log("Game running");
    repaint();
    while (!exiting) {
	    Uint64 t = SDL_GetTicks64();
	    Uint64 dt = t - lastTime;
        if (dt < RENDER_QUANTUM/2) {
            SDL_Delay(RENDER_QUANTUM/2 - dt);
            t = SDL_GetTicks64();
            dt = t - lastTime;
            continue;
        }
    	currentScene -> update(dt);
        ticks++;
        lastTime = t;
    
    	if (t - lastDrawTime >= RENDER_QUANTUM) {
        	repaint();
        	frames++;
        	lastDrawTime = t;
    	}

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exiting = true;
                break;
            } else {
                if (event.type == SDL_USEREVENT) {
                    ;
                } else {
                    currentScene -> handleEvent(event);
                }
            }
        }
    }
    std::cout << ticks << " ticks and "
      << frames << " frames in " 
      << lastTime << "ms, @" 
      << 1000.0 * ticks / lastTime << " ticks per sec, " 
      << 1000.0 * frames / lastTime << " fps\n";
}

void Game::repaint(){
    SDL_RenderClear(engine.renderer);
    for (auto& e : currentScene->entities) {
        if (e != currentScene->entities[0]){
            engine.draw(e);
        } else {
            engine.draw(e, (int)e->pos.x, (int)e->pos.y, WIDTH, HEIGHT);
            engine.draw(e, (int)e->pos.x + WIDTH, (int)e->pos.y, WIDTH, HEIGHT);
        }
    }   
    SDL_RenderPresent(engine.renderer);
}

int main(){
    Game game;
    game.run();
    return 0;
}
