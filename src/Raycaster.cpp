#include "Raycaster.hpp"
#include "ARGBSurface.hpp"
#include "Assets.hpp"

#include <cmath>
#include <iostream>

Raycaster::Raycaster() {
    this->level = NULL;
    this->posX = 0.0;
    this->posY = 0.0;
    this->dirX = 0.0;
    this->dirY = 0.0;
    this->planeX = 0.0;
    this->planeY = 0.0;
}

void Raycaster::cast(SDL_Surface *screen, unsigned scale) {
    if(SDL_MUSTLOCK(screen)) {
        if(!SDL_LockSurface(screen)) {
            return;
        }
    }
    for(unsigned x = 0; x < screen->w / scale; x++) {
        this->cast_ray(screen, scale, x);
    }
    if(SDL_MUSTLOCK(screen)) {
        SDL_UnlockSurface(screen);
    }
}

void Raycaster::cast_ray(SDL_Surface *screen, unsigned scale, unsigned column) {
    unsigned w = screen->w / scale;
    unsigned h = screen->h / scale;

    double camX = 2 * column / (double)w - 1;
    double rDirX = dirX + planeX * camX;
    double rDirY = dirY + planeY * camX;
    unsigned mapX = (unsigned) posX;
    unsigned mapY = (unsigned) posY;

    double sideDistX, sideDistY;
    double deltaX = sqrt(1 + (rDirY * rDirY) / (rDirX * rDirX));
    double deltaY = sqrt(1 + (rDirX * rDirX) / (rDirY * rDirY));

    double perpWallDist;

    unsigned stepX, stepY;
    bool hit = false;
    unsigned side = 0;

    if(rDirX < 0) {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaX;
    }
    else {
        stepX = +1;
        sideDistX = (mapX + 1.0 - posX) * deltaX;
    }
    if(rDirY < 0) {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaY;
    }
    else {
        stepY = +1;
        sideDistY = (mapY + 1.0 - posY) * deltaY;
    }

    while(!hit && mapX >= 0 && mapX < level->width() && mapY >= 0 && mapY < level->height()) {
        if(sideDistX < sideDistY) {
            sideDistX += deltaX;
            mapX += stepX;
            side = 0;
        }
        else {
            sideDistY += deltaY;
            mapY += stepY;
            side = 1;
        }

        if(level->get(mapX, mapY, Level::Wall) > 0) hit = true;
    }

    if(side == 0) {
        perpWallDist = fabs((mapX - posX + (1 - stepX) / 2) / rDirX);
    }
    else {
        perpWallDist = fabs((mapY - posY + (1 - stepY) / 2) / rDirY);
    }

    unsigned height = abs(h / perpWallDist);

    unsigned start = (h - height) / 2;
    unsigned end = (h + height) / 2;

    if(start < 0) start = 0;
    if(end >= h) end = h - 1;

    ARGBSurface tex = Assets::wall(level->get(mapX, mapY, Level::Wall));
    unsigned texW = tex.surf->w;
    unsigned texH = tex.surf->h;

    double wallX = 0;
    if(side == 1) {
        wallX = posX + ((mapY - posY + (1 - stepY) / 2) / rDirY) * rDirX;
    }
    else {
        wallX = posY + ((mapX - posX + (1 - stepX) / 2) / rDirX) * rDirY;
    }

    wallX -= floor(wallX);

    int texX = (int) (wallX * texW);
    if(side == 0 && rDirX > 0) texX = texW - texX - 1;
    else if(side == 1 && rDirY < 0) texX = texW - texX - 1;

    for(unsigned y = start; y < end; y++) {
        unsigned d = y * 256 - h * 128 + height * 128;
        int texY = ((d * texH) / height) / 256;
        u32 color = tex.get(texX, texY);
        if(side == 1)
            color = (color >> 1) & 0xff7f7f7f;

        SDL_Rect pixel = {(i16) (column * scale), (i16)(y * scale), (u16)scale, (u16)scale};
        SDL_FillRect(screen, &pixel, color);
    }

    // zbuffer[column] = perpWallDist;

    // Cast floor and ceiling
    double fXWall = 0.0, fYWall = 0.0;

    if(side == 0 && rDirX > 0) {
        fXWall = mapX;
        fYWall = mapY + wallX;
    }
    else if(side == 0 && rDirX < 0) {
        fXWall = mapX + 1.0;
        fYWall = mapY + wallX;
    }
    else if(side == 1 && rDirY > 0) {
        fXWall = mapX + wallX;
        fYWall = mapY;
    }
    else {
        fXWall = mapX + wallX;
        fYWall = mapY + 1.0;
    }

    double distWall = perpWallDist;
    double currentDist = 0.0;

    // TODO Allow for different ceiling/floor types
    ARGBSurface ceil = Assets::ceiling(0);
    ARGBSurface floor = Assets::floor(0);
    unsigned ftexW = floor.surf->w, ftexH = floor.surf->h;
    unsigned ctexW = ceil.surf->w, ctexH = ceil.surf->h;

    for(unsigned y = end; y < h + 1; y++) {
        currentDist = h / (2.0 * y - h);
        double weight = currentDist / distWall;
        double curX = weight * fXWall + (1.0 - weight) * posX;
        double curY = weight * fYWall + (1.0 - weight) * posY;

        int fTexX = (int) (curX * ftexW) % ftexW;
        // Floor texture should be flipped since we're drawing it upside down
        int fTexY = ftexH - ((int) (curY * ftexH) % ftexH);
        int cTexX = (int) (curX * ctexW) % ctexW;
        int cTexY = (int) (curY * ctexH) % ctexH;

        u32 fCol = floor.get(fTexX, fTexY);
        u32 cCol = ceil.get(cTexX, cTexY);

        // TODO calculate fog

        SDL_Rect fpixel = {(i16) (column * scale), (i16)(y * scale), (u16)scale, (u16)scale};
        SDL_FillRect(screen, &fpixel, fCol);
        SDL_Rect cpixel = {(i16) (column * scale), (i16)((h - y) * scale), (u16)scale, (u16)scale};
        SDL_FillRect(screen, &cpixel, cCol);
    }
}

void Raycaster::dir(double ang) {
    dirX = -cos(-ang);
    dirY = -sin(-ang);
}

void Raycaster::plane(double ang) {
    planeX = -0.66 * sin(-ang);
    planeY = 0.66 * cos(-ang);
}
