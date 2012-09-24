#include "SDL/SDL.h"

#include "ARGBSurface.hpp"
#include "IntDefs.hpp"

ARGBSurface::ARGBSurface(int width, int height) {
    this->surf = SDL_CreateRGBSurface(SDL_SRCALPHA, width, height, 32, 0, 0, 0, 0);
}

ARGBSurface::ARGBSurface(SDL_Surface *image) {
    SDL_PixelFormat *fmt = new SDL_PixelFormat;
    fmt->palette = NULL;
    fmt->BitsPerPixel = 32;
    fmt->BytesPerPixel = 4;
    fmt->Rloss = fmt->Gloss = fmt->Bloss = fmt->Aloss = 0;
    fmt->Rmask = 0x00FF0000;
    fmt->Gmask = 0x0000FF00;
    fmt->Bmask = 0x000000FF;
    fmt->Amask = 0xFF000000;
    fmt->colorkey = 0x00FF00FF;
    fmt->alpha = 0xFF;
    this->surf = SDL_ConvertSurface(image, fmt, SDL_SRCALPHA);
}

u32 ARGBSurface::get(int x, int y) {
    if(this->surf == NULL)
        return 0;
    if(x < 0 || y < 0 || x >= this->surf->w || y >= this->surf->h)
        return 0;
    u8 *p = (u8 *)this->surf->pixels + y * this->surf->pitch + x * 4;
    return *(u32 *)p;
}

u32 ARGBSurface::operator()(int x, int y) {
    return this->get(x, y);
}

bool ARGBSurface::set(int x, int y, u32 color) {
    if(this->surf == NULL)
        return false;
    if(x < 0 || y < 0 || x >= this->surf->w || y >= this->surf->h)
        return false;
    u8 *p = (u8 *)this->surf->pixels + y * this->surf->pitch + x * 4;
    *(u32 *)p = color;
    return true;
}

