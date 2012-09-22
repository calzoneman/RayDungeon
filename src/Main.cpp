#include "SDL/SDL.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
    if(SDL_Init( SDL_INIT_EVERYTHING ) < 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    SDL_Surface *screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
    if(!screen) {
        std::cout << "Error setting video mode: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    SDL_WM_SetCaption("RayDungeon", 0);
    while(true) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                SDL_Quit();
                return EXIT_SUCCESS;
            }
        }
    }
    return EXIT_SUCCESS;
}
