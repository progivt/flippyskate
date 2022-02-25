#include "game.hpp"

#define WIDTH  800
#define HEIGHT 600
#define RENDER_QUANTUM 16

Game::Game() 
  : engine { Engine(WIDTH, HEIGHT) },
    level { Level(WIDTH, HEIGHT) } { 
    currentScene = &level;
    score = highScore = lastScore = 0;
    level.scorecard.text = "0";
    loadTextures(currentScene);
    
    repaint();
    ticks = 0; 
    frames = 1;
    lastDrawTime = lastTime = SDL_GetTicks64();
    SDL_Log("Game init ok");
}

void Game::loadTextures(Scene* scene){
	for (auto& e : currentScene -> entities) {
		engine.loadEntityTexture(e);
	}
}

void Game::invalidateScore(){
    if (currentScene != &level || score == lastScore) 
        return;

    level.scorecard.text = std::to_string(score);
    SDL_DestroyTexture(level.scorecard.texture.sdlTexture);
    level.scorecard.texture.sdlTexture = nullptr;
    engine.loadEntityTexture(&level.scorecard);
    level.scorecard.px = WIDTH - level.scorecard.texture.w - 20;
    lastScore = score;
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
    score++;
    invalidateScore();

    SDL_Renderer* rend = engine.renderer;
    SDL_RenderClear(rend);
    for (auto& e : currentScene->entities) {
        engine.draw(e);
        if (e == &currentScene->bg) {
            engine.draw(e, e->px+WIDTH, e->py);
        }
    }
    SDL_RenderPresent(rend);
}

int main(){
    Game game;
    game.run();
    return 0;
}