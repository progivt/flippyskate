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
void Engine::loadEntityTexture(Entity* e) {
    if (e->name[0] != TXTMARK[0]){
        // графическая текстура из файла, кэшируется в мапе "имя -> Texture"
        SDL_Log("Getting texture: <%s>", e->name);
        if (images.find(e->name) == images.end()) {
            std::string path = IMG_LOCATION;
            path = path + e->name + ".png";
            
            SDL_Texture* sdlTexture;
            if ((sdlTexture = IMG_LoadTexture(renderer, path.c_str())) != NULL) {
                int w, h;
                SDL_QueryTexture(sdlTexture, NULL, NULL, &w, &h);
                images[e->name] = Texture {w, h, sdlTexture};
                SDL_Log("  Texture <%s> loaded from %s, w=%d, h=%d", e->name, path.c_str(), w, h);
            }
        } else {
            SDL_Log("  Texture <%s> found in cache", e->name);
        }
        e->texture = &images[e->name];
    } else {
        // сгенерировать текстуру, отрисовав текст, находящийся в поле text
        SDL_Surface *surface = TTF_RenderText_Solid(font, e->text.c_str(), e->textColor);
        if (e->texture != nullptr) {
            if (e->texture->sdlTexture != nullptr) {
                SDL_DestroyTexture(e->texture->sdlTexture);
                SDL_Log("Destroyed text texture");
            }
        } else {
            e->texture = new Texture;
        }
        e->texture->w = surface->w;
        e->texture->h = surface->h;
        e->texture->sdlTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_Log("Text \"%s\" rendered", e->text.c_str());
    }
    e->srcRect.w = e->texture->w;
    e->srcRect.h = e->texture->h;
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
// "возьми из srcRect""
void Engine::draw(Entity* e, int x, int y, int w, int h){
    if (!e->visible) return;
    if (e->texture == nullptr || e->texture->sdlTexture == nullptr) {
        SDL_Log("Request to draw %s which has no texture", e->name);
        loadEntityTexture(e);
    }
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
        SDL_Rect dstRect {x, y, w, h};
        SDL_RenderCopy(renderer, e->texture->sdlTexture, &srcRect, &dstRect);
    }
}

Engine::~Engine() {
    // почистить текстуры для картинок
    for (auto& [name, Texture] : images) {
        SDL_Log("Destroying texture %s", name);
        SDL_DestroyTexture(Texture.sdlTexture);
    }
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_Delay(500);
    SDL_Quit();
}
