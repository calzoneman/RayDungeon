#include "Level.hpp"
#include "ARGBSurface.hpp"
#include "Raycaster.hpp"
#include "Assets.hpp"

#include "SDL/SDL.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
    if(SDL_Init( SDL_INIT_EVERYTHING ) < 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    SDL_Surface *screen = SDL_SetVideoMode(1024, 768, 32, SDL_HWSURFACE | SDL_SRCALPHA | SDL_DOUBLEBUF);
    if(!screen) {
        std::cout << "Error setting video mode: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    SDL_WM_SetCaption("RayDungeon", 0);

    Assets::init();

    Level level(16, 16);
    Raycaster raycaster;
    raycaster.level = &level;
    raycaster.posX = 8.0;
    raycaster.posY = 8.0;
    double ang = 0.0;
    unsigned time = SDL_GetTicks();
    int frames = 0;
    while(true) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                SDL_Quit();
                return EXIT_SUCCESS;
            }
        }
        raycaster.dir(ang);
        raycaster.plane(ang);
        ang += 0.001;
        raycaster.cast(screen, 4);
        SDL_Flip(screen);
        if(SDL_GetTicks() > time + 1000) {
            std::cout << "FPS: " << frames << std::endl;
            frames = 0;
            time = SDL_GetTicks();
        }
        frames++;
    }
    return EXIT_SUCCESS;
}
