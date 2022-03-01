#include "game.hpp"

#define WIDTH  1024
#define HEIGHT 600
#define RENDER_QUANTUM 16

Game::Game() 
  : engine { Engine(WIDTH, HEIGHT) },
    level { Level(WIDTH, HEIGHT) },
    welcomeScreen { WelcomeScreen (WIDTH, HEIGHT) } {
    
    welcomeScreen.start.textColor = SDL_Color {255,255,255,0};
    welcomeScreen.start.pos.x = (WIDTH - welcomeScreen.start.srcRect.w)/2;
    welcomeScreen.start.pos.y = (HEIGHT - welcomeScreen.start.srcRect.h)/2;

    highScore = lastScore = 0;
    level.scorecard.text = "0";
    currentScene = &level;
    loadTextures(currentScene);
    
    repaint();
    ticks = 0; 
    frames = 1;
    lastDrawTime = lastTime = SDL_GetTicks64();
    SDL_Log("Game init ok");
}

void Game::loadTextures(Scene* scene){
	for (auto& e : currentScene -> entities)
		engine.loadEntityTexture(e);
}

void Game::invalidateScore(){
    if (currentScene != &level || level.score == lastScore) 
        return;
    Entity *sc = &level.scorecard;
    sc->text = std::to_string(level.score);
    if (sc->texture.sdlTexture != nullptr) {
        SDL_DestroyTexture(sc->texture.sdlTexture);
        sc->texture.sdlTexture = nullptr;
    }
    engine.loadEntityTexture(sc);
    sc->pos.x = WIDTH - sc->texture.w - 20;
    lastScore = level.score;
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
    level.score++;
    invalidateScore();

    SDL_RenderClear(engine.renderer);
    for (auto& e : currentScene->entities) {
        if (e != &currentScene->bg){
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
