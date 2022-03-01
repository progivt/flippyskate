#include <iostream>
#include <memory>
#include <string>

#include "engine.hpp"

#define IMG_LOCATION "./res/"
#define FONT_FILE "./res/BalonkuRegular.otf"
#define FONT_SIZE 48

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
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W, H,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    SDL_Log("Renderer %s: accelerated %d, VSynced %d", 
        info.name,
        (bool)(info.flags & SDL_RENDERER_ACCELERATED),
        (bool)(info.flags & SDL_RENDERER_PRESENTVSYNC));

    if (TTF_Init()) 
        die ("SDL_ttf init error: %s\n", TTF_GetError());
    
    if ((font = TTF_OpenFont(FONT_FILE, FONT_SIZE)) == NULL)
        die("Could not load TTF font\n");

    SDL_Log("Engine init ok");
}

// загружает текстуру из файла для Entity, или отрисовывает текст 
// проверяет текстуры в мапе имя -> текстура и добавляет, если  ее там еще нет
void Engine::loadEntityTexture(Entity* e) {
    if (e->texture.sdlTexture != nullptr) 
        return;
    if (e->name[0] != TXTMARK){
        // графическая текстура из файла, 
        if (images.find(e->name) == images.end()) {
            SDL_Texture* texture;
            std::string path = IMG_LOCATION;
            path = path + e->name + ".png";

            if ((texture = IMG_LoadTexture(renderer, path.c_str())) != NULL) {
                int w, h;
                // получить и запомнить размеры текстуры
                SDL_QueryTexture(texture, NULL, NULL, &w, &h);
                images[e->name] = Texture {w, h, texture};
                e->texture = images[e->name];
                SDL_Log("texture %s loaded, w=%d, h=%d", path.c_str(), e->texture.w, e->texture.h );
            }
        }
        e->texture = images[e->name];
    } else {
        // отрисовать текст, находящийся в Entity в поле text,
        // в текстуру
        SDL_Surface *surface = TTF_RenderText_Solid(
            font, e->text.c_str(), e->textColor);
        e->texture.sdlTexture = SDL_CreateTextureFromSurface(renderer, surface);
        e->texture.w = surface->w;
        e->texture.h = surface->h;
        SDL_FreeSurface(surface);
    }
    e->srcRect.w = e->texture.w;
    e->srcRect.h = e->texture.h;
}

// рисует Entity в координатах, хранящихся в нем
void Engine::draw(Entity* e){
    draw(e, e->pos);
}

// рисует Entity в заданной позиции в масштабе 1:1
void Engine::draw(Entity* e, vec2 pos){
    draw(e, pos.x, pos.y, 0, 0);
}

// рисует Entity растягивая на заданный прямоугольник
// с x-обрезкой по ширине окна. Размеры 0 означают
// "возьми из исходника""
void Engine::draw(Entity* e, int x, int y, int w, int h){
    int margin = 0;
    SDL_Rect srcRect {e->srcRect};
    if (w==0) w = srcRect.w;
    if (h==0) h = srcRect.h;
    float xratio = (w > 0) ? 1.*srcRect.w/w : 1, 
          yratio = (h > 0) ? 1.*srcRect.h/h : 1;
    if ((margin = x + w - W) > 0) {
        w -= margin;
        srcRect.w -= xratio * margin;
    } 
    if ((margin = -x) > 0) {
        x = 0;
        w -= margin;
        srcRect.x += xratio * margin;
        srcRect.w -= xratio * margin;
    }
    if (srcRect.w > 0) {
        if (e->texture.sdlTexture == nullptr)
            loadEntityTexture(e);
        SDL_Rect dstRect {x, y, w, h};
        SDL_RenderCopy(renderer, e->texture.sdlTexture, &srcRect, &dstRect);
    }
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
