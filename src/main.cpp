#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <glad/glad.h>

SDL_Surface* loadSurface(const char* path, SDL_Surface* screenSurface) {
    SDL_Surface *optimizedSurface = NULL, 
                *loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL)
        std::cerr << "Unable to load image " << path 
                  << " SDL_image Error: " << IMG_GetError();
    else {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0);
        if (optimizedSurface == NULL)
            std::cerr << "Unable to optimize image " << path 
                      << " SDL Error: " << SDL_GetError();
        SDL_FreeSurface(loadedSurface);
    }
    return optimizedSurface;
}

int main(int argc, char* argv[]) {

    SDL_Window* window = nullptr;
    SDL_Surface *screen, *bg;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init error: " << SDL_GetError();
        return 1;
    } 
    
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    // SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

    window = SDL_CreateWindow("Flappy Skater",
                              20, 20,
                              640, 480,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(window);

    // Setup our function pointers
    gladLoadGLLoader(SDL_GL_GetProcAddress);

    //SDL_Surface* bg = loadSurface("res/bg1.jpeg", screen);

    screen = SDL_GetWindowSurface(window);
    
    SDL_Surface* image;
    // image = SDL_LoadBMP("./res/bg1.bmp");
    image = loadSurface("res/bg1.jpeg", screen);

    SDL_BlitSurface(image, NULL, screen, NULL);
    SDL_FreeSurface(image);
    SDL_UpdateWindowSurface(window);

    SDL_Event event;
    event.type = SDL_FIRSTEVENT;

    bool newPress = true;
    while (event.type != SDL_QUIT) {
        
        glViewport(0, 0, 640, 480);

        while (SDL_PollEvent(&event)){
            // Handle each specific event
            if (event.type == SDL_QUIT) break;
            
            if (event.type == SDL_MOUSEMOTION) {
                std::cout << "mouse has been moved\n";
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && newPress) {
                    std::cout << "space was pressed\n";
                    newPress = false;
            }
            if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
                    newPress = true;
            }
        }

        glClearColor(1.0f,0.0f,0.0f,1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Delay(500);
    SDL_Quit();
    return 0;
}
