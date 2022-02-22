#include <iostream>
#include "game.hpp"

void die(const char* msg1, const char* msg2, int errorCode=1){
	std::cerr << msg1 << msg2;
	exit(errorCode);
}

Game::Game(int width, int height, const char* windowTitle){
    // Иницаилизация SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    	die("SDL init error: ", SDL_GetError());
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

    if (!IMG_Init(IMG_INIT_PNG)) {
    	die("SDL init error: ", SDL_GetError());
    }

	window = SDL_CreateWindow(windowTitle,
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              width, height,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void Game::LoadSprite(const char* path){ 
    sprites.push_back(Sprite(renderer, path));
}

void Game::CreateEntity(Sprite sprite, float _px, float _py, 
                      float _vx, float _vy, float _ax, float _ay){
    entities.push_back(Entity(renderer, sprite, _px, _py, _vx, _vy, _ax, _ay));
}

void Game::Tick(Uint64 dt){
    for (auto& e : entities){
        e.Tick(dt);
    }
}

void Game::draw(){
    SDL_RenderClear(renderer);
    for (auto& e : entities){
        e.draw();
    }
    SDL_RenderPresent(renderer);
}

Game::~Game(){
    for (auto& s : sprites){
        SDL_DestroyTexture(s.texture);
    }
	SDL_DestroyWindow(window);
    SDL_Delay(500);
    SDL_Quit();
}
