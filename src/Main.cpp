#include "Level.hpp"
#include "ARGBSurface.hpp"
#include "Raycaster.hpp"

#include "SDL/SDL.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
    if(SDL_Init( SDL_INIT_EVERYTHING ) < 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    SDL_Surface *screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_SRCALPHA);
    if(!screen) {
        std::cout << "Error setting video mode: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    SDL_WM_SetCaption("RayDungeon", 0);

    SDL_Surface *temp;
    ARGBSurface *img;
    temp = SDL_LoadBMP("test.bmp");
    if(!temp) {
        std::cout << "Unable to load bitmap: " << SDL_GetError() << std::endl;
    }
    img = new ARGBSurface(temp);
    SDL_FreeSurface(temp);

    Level level(16, 16);
    Raycaster raycaster;
    raycaster.level = &level;
    raycaster.posX = 8.0;
    raycaster.posY = 8.0;
    double ang = 0.0;
    int time = SDL_GetTicks();
    int frames = 0;
    while(true) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                SDL_Quit();
                return EXIT_SUCCESS;
            }
        }
        SDL_Rect src = {0, 0, (u16)img->surf->w, (u16)img->surf->h};
        SDL_Rect dest = {100, 100, (u16)img->surf->w, (u16)img->surf->h};
        SDL_BlitSurface(img->surf, &src, screen, &dest);
        raycaster.dir(ang);
        raycaster.plane(ang);
        ang += 0.01;
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
