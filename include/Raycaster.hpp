#ifndef RAYCASTER_HPP_INCLUDED
#include "SDL/SDL.h"

#include "Level.hpp"

class Raycaster {
    public:
        Raycaster();
        Level *level;
        double posX, posY;
        double dirX, dirY;
        double planeX, planeY;

        void cast(SDL_Surface *screen, unsigned scale);
        void cast_ray(SDL_Surface *screen, unsigned scale, unsigned column);
};

#define RAYCASTER_HPP_INCLUDED
#endif
