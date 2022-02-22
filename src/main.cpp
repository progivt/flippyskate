    #include <iostream>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <glad/glad.h>

SDL_Surface* loadSurface(const char* path, SDL_Surface* screenSurface) {
    SDL_Surface* optimizedSurface = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path);
    if( loadedSurface == NULL ) 
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    else {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, screenSurface->format, 0 );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError() );
        }
        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}

int main(int argc, char* argv[]) {

    SDL_Window* window = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init error: " << SDL_GetError();
        return 1;
    } 

    //Initialize PNG loading
    int imgFlags = IMG_INIT_JPG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        std::cerr << "SDL_image init error: " <<  IMG_GetError();
        return 1;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

    window = SDL_CreateWindow("Flappy Skater",
                              20,20,
                              640,480,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    // Setup our function pointers

    SDL_GLContext context = SDL_GL_CreateContext(window);

    SDL_Surface* screen = SDL_GetWindowSurface(window);
    gladLoadGLLoader(SDL_GL_GetProcAddress);

    //SDL_Surface* bg = loadSurface("res/bg1.jpeg", screen);

    // Infinite loop for our application
    SDL_Event event;
    event.type = SDL_FIRSTEVENT;

    while(event.type != SDL_QUIT){
        glViewport(0, 0, 640, 480);

        // Start our event loop
        while (SDL_PollEvent(&event)){
            // Handle each specific event
            if (event.type == SDL_QUIT) break;
            
            if (event.type == SDL_MOUSEMOTION) {
                std::cout << "mouse has been moved\n";
            }
            if (event.type == SDL_KEYDOWN) {
                std::cout << "a key has been pressed\n";
                if (event.key.keysym.sym == SDLK_0) {
                    std::cout << "0 was pressed\n";
                } else {
                    std::cout << "0 was not pressed\n";
                }
            }
            // Retrieve the state of all of the keys
            // Then we can query the scan code of one or more
            // keys at a time
            const Uint8* state = SDL_GetKeyboardState(NULL);
            if (state[SDL_SCANCODE_RIGHT]) {
                std::cout << "right arrow key is pressed\n";
            }
        }

        glClearColor(1.0f,0.0f,0.0f,1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Delay(1000);
    SDL_Quit();
    return 0;
}
