#include "game.hpp"

#define WIDTH  800
#define HEIGHT 600
#define RENDER_QUANTUM 16

Game::Game() 
  : engine { Engine(WIDTH, HEIGHT) },
    gameScreen { GameScreen(&engine) },
    welcomeScreen { WelcomeScreen (&engine) } {
        
    scenes = std::vector<Scene *> {&welcomeScreen, &gameScreen};
    
    welcomeScreen.start.textColor = SDL_Color {255,255,255,0};
    welcomeScreen.start.pos.x = (WIDTH  - welcomeScreen.start.srcRect.w) / 2;
    welcomeScreen.start.pos.y = (HEIGHT - welcomeScreen.start.srcRect.h) / 2;

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

Game::~Game() {
    // почистить динамические текстуры для текста
    for (auto& scene : scenes)
        for (auto& e: scene->entities)
            if (e->name[0] == TXTMARK[0]) {
                SDL_Log("Destroying text %s", e->text.c_str());
                if (e->texture && e->texture->sdlTexture) {
                    SDL_DestroyTexture(e->texture->sdlTexture);
                    SDL_Log("Destroyed text texture for text \"%s\"", e->text.c_str());
                }
                delete e->texture;
            }
}

int main(){
    Game game;
    game.run();
    return 0;
}
