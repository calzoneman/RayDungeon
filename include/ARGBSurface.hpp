#ifndef ARGBSURFACE_HPP_INCLUDED

#include "SDL/SDL.h"

#include "IntDefs.hpp"

class ARGBSurface {
    public:
        SDL_Surface *surf;

        ARGBSurface(int width, int height);
        ARGBSurface(SDL_Surface *image);

        u32 get(int x, int y);
        u32 operator()(int x, int y);
        bool set(int x, int y, u32 color);
};

#define ARGBSURFACE_HPP_INCLUDED
#endif
