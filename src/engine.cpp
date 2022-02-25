#include <iostream>
#include <memory>
#include <string>

#include "engine.hpp"

#define IMG_LOCATION "./res/"

void die(const char* msg1, const void* msg2=NULL) {
    SDL_Log(msg1, msg2);
    exit(1);
}

Engine::Engine(int width, int height) 
    : W {width}, H {height} {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
        die("SDL init error: %s\n", SDL_GetError());    
    
    if (IMG_Init(IMG_INIT_PNG) ^ IMG_INIT_PNG) 
        die("SDL_image init error: %s\n", IMG_GetError());
    
    if (SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl")!=SDL_TRUE)
        SDL_Log("Could not set hint for OpenGL: %s\n", IMG_GetError());    

    window = SDL_CreateWindow("Flappy Skater",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                              W, H,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    SDL_Log("Renderer %s", info.name);
    SDL_Log("Accelerated %d, VSynced %d", 
        (bool)(info.flags & SDL_RENDERER_ACCELERATED),
        (bool)(info.flags & SDL_RENDERER_PRESENTVSYNC));
}

// загружает и сохраняет текстуры по имени файла в мапе имя -> текстура
Texture Engine::getTexture(const char* filename) {
    if (images.find(filename) == images.end()) {
        SDL_Texture* texture;
        std::string path = IMG_LOCATION;
        path = path + filename + ".png";

        if ((texture = IMG_LoadTexture(renderer, path.c_str())) != NULL) {
            int w, h;
            // получить и запомнить размеры текстуры
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            images[filename] = Texture {w, h, texture};
        }
    }
    return images[filename];
}

// загружает текстуру для Entity, если она еще не загружена
void Engine::loadEntityTexture(Entity* e) {
    SDL_Log("Loading texture for entity: %s", e->texture.imgFileName);
    if (e->texture.sdlTexture == nullptr){
        e->texture = getTexture(e->texture.imgFileName);
    }
    e->srcRect.w = e->texture.w;
    e->srcRect.h = e->texture.h;
}

// рисует Entity в координатах, хранящихся в нем
void Engine::draw(Entity* e){
    draw(e, e->px, e->py);
}

// рисует Entity в заданном месте
// с x-обрезкой по ширине окна
void Engine::draw(Entity* e, float _px, float _py){
    SDL_Rect srcRect {e->srcRect};
    SDL_Rect dstRect {(int)_px, (int)_py, srcRect.w, srcRect.h};
    int margin;
    if ((margin = dstRect.x + dstRect.w - W) > 0) {
        dstRect.w -= margin;
        srcRect.w -= margin;
    } 
    if ((margin = -dstRect.x) > 0) {
        dstRect.x = 0;
        dstRect.w -= margin;
        srcRect.x += margin;
        srcRect.w -= margin;
    }
    if (srcRect.w > 0)
        SDL_RenderCopy(renderer, e->texture.sdlTexture, &srcRect, &dstRect);
}

Engine::~Engine() {
    for (auto& [name, Texture] : images) {
        SDL_DestroyTexture(Texture.sdlTexture);
    }
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_Delay(500);
    SDL_Quit();
}
