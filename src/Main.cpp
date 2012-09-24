#include "Level.hpp"
#include "ARGBSurface.hpp"

#include "SDL/SDL.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
    if(SDL_Init( SDL_INIT_EVERYTHING ) < 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    SDL_Surface *screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_SRCALPHA);
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
    std::cout << level(10, 10, Level::Wall) << std::endl;
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
        SDL_Flip(screen);
    }
    return EXIT_SUCCESS;
}
