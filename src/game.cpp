#include "game.hpp"

#define WIDTH  800
#define HEIGHT 600
#define RENDER_QUANTUM 16

Game::Game() 
  : engine { Engine(WIDTH, HEIGHT) },
    level { Level(WIDTH, HEIGHT) } { 
    currentScene = &level;
    loadTextures(currentScene);
    
    ticks = 0; 
    repaint();
    frames = 1;
    lastDrawTime = lastTime = SDL_GetTicks64();

    score = highScore = 0;
    SDL_Log("Game init ok");
}

void Game::loadTextures(Scene* scene){
	for (auto& e : currentScene -> entities) {
		engine.loadEntityTexture(e);
	}
}

void Game::run(){
    bool exiting = false;
    SDL_Log("Starting game");
    repaint();
    while (!exiting) {
	    Uint64 t = SDL_GetTicks64();
	    Uint64 dt = t - lastTime;
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
                currentScene -> handleEvent(event);
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
    SDL_Renderer* rend = engine.renderer;
    SDL_RenderClear(rend);
    for (auto& e : currentScene->entities) {
        engine.draw(e);
        if (e == &currentScene->bg) {
            engine.draw(e, e->px+WIDTH, e->py);
        }
    }

    if (currentScene == &level) {        
        strScore.str("");
        strScore << score;
        SDL_Color textColor = {0, 0, 33, 0};
        SDL_Surface *surface = TTF_RenderText_Solid(
            engine.font, strScore.str().c_str(), textColor);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(
            engine.renderer, surface);
        SDL_Rect scoreSrc {0, 0, surface->w, surface->h};
        SDL_Rect scoreDst {WIDTH - surface->w - 20, 10, scoreSrc.w, scoreSrc.h};
        SDL_RenderCopy(engine.renderer, texture, NULL, &scoreDst);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        score++;
    }

    SDL_RenderPresent(rend);
}

