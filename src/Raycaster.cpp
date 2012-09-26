#include "Raycaster.hpp"

#include <cmath>

Raycaster::Raycaster() {
    this->level = NULL;
    this->posX = 0.0;
    this->posY = 0.0;
    this->dirX = 0.0;
    this->dirY = 0.0;
    this->planeX = 0.0;
    this->planeY = 0.0;
}

void Raycaster::cast_ray(SDL_Surface *screen, unsigned scale, unsigned column) {
    int offY = 0;
    unsigned w = screen->w;
    unsigned h = screen->h;

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

    while(!hit && mapX > 0 && mapX < level->width() && mapY >= 0 && mapY < level->height()) {
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

    // TODO Get texture
    unsigned texW = 8;
    unsigned texH = 8;

    double wallX = 0;
    if(side == 1) {
        wallX = posX + ((mapY - posY + (1 - stepY) / 2) / rDirY) * rDirX;
    }
    else {
        wallX = posY + ((mapX - posX + (1 - stepX) / 2) / rDirX) * rDirY;
    }

    wallX -= floor(wallX);

    unsigned texX = (unsigned) (wallX * texW);
    if(side == 0 && rDirX > 0) texX = texW - texX - 1;
    else if(side == 1 && rDirY < 0) texX = texW - texX - 1;

    if(end + offY >= 0 && start + offY < h) {
        for(unsigned y = start; y <= end; y++) {
            if(y + offY < 0)
                continue;
            if(y + offY >= h)
                break;
            unsigned d = y * 256 - h * 128 + height * 128;
            unsigned texY = ((d * texH) / height) / 256;
            // TODO get color from texture
            unsigned color = 0xFF000000;
            if(side == 1) {
                color = (color >> 1) & 0x7f7f7f;
            }
            SDL_Rect pixel = {(i16) (column * scale), (i16)((y + offY) * scale), (u16)scale, (u16)scale};
            SDL_FillRect(screen, &pixel, color);
        }
    }

    zbuffer[column] = perpWallDist;

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

    // TODO Load textures
    unsigned ftexW = 0, ftexH = 0;
    unsigned ctexW = 0, ctexH = 0;

    for(unsigned y = end + 1; y - abs(offY) < h + 1; y++) {
        // What the fuck?
        // I have no idea what these 2 lines mean but it's for vertical rotation
        bool yc = y + offY >= h || y + offY < 0;
        bool hyc = h - y + offY >= h || h - y + offY < 0;

        if(yc && hyc)
            continue;

        currentDist = h / (2.0 * y - h);
        double weight = currentDist / distWall;
        double curX = weight * fXWall + (1.0 - weight) * posX;
        double curY = weight * fYWall + (1.0 - weight) * posY;

        unsigned fTexX = (unsigned) (curX * ftexW) % ftexW;
        unsigned fTexY = (unsigned) (curY * ftexH) % ftexH;
        unsigned cTexX = (unsigned) (curX * ctexW) % ctexW;
        unsigned cTexY = (unsigned) (curY * ctexH) % ctexH;

        // TODO Load from textures
        unsigned fCol = 0xFF00FF00;
        unsigned cCol = 0xFF00FFFF;

        // TODO calculate fog

        if(y + offY >= 0 && y + offY < h) {
            SDL_Rect pixel = {(i16) (column * scale), (i16)((y + offY) * scale), (u16)scale, (u16)scale};
            SDL_FillRect(screen, &pixel, fCol);
        }
        if(h - y + offY >= 0 && h - y + offY < h) {
            SDL_Rect pixel = {(i16) (column * scale), (i16)((h - y + offY) * scale), (u16)scale, (u16)scale};
            SDL_FillRect(screen, &pixel, cCol);
        }
    }

}
