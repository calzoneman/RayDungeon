#include "SDL/SDL.h"
#include <iostream>

#include "Assets.hpp"
#include "ARGBSurface.hpp"

namespace Assets {

    std::vector<ARGBSurface> floors;
    std::vector<ARGBSurface> walls;
    std::vector<ARGBSurface> ceilings;

    void init() {
        SDL_Surface *temp;
        temp = SDL_LoadBMP("floor.bmp");
        if(temp)
            floors.push_back(ARGBSurface(temp));
        else
            floors.push_back(ARGBSurface(16, 16));
        SDL_FreeSurface(temp);

        // Placeholder for id 0
        walls.push_back(ARGBSurface(16, 16));
        temp = SDL_LoadBMP("wall.bmp");
        if(temp)
            walls.push_back(ARGBSurface(temp));
        else
            walls.push_back(ARGBSurface(16, 16));
        SDL_FreeSurface(temp);
        std::cout << walls.size() << std::endl;

        temp = SDL_LoadBMP("ceiling.bmp");
        if(temp)
            ceilings.push_back(ARGBSurface(temp));
        else
            ceilings.push_back(ARGBSurface(16, 16));
        SDL_FreeSurface(temp);
    }

    ARGBSurface floor(unsigned id) {
        if(id >= floors.size())
            return ARGBSurface(16, 16);
        return floors[id];
    }

    ARGBSurface wall(unsigned id) {
        if(id >= walls.size())
            return ARGBSurface(16, 16);
        return walls[id];
    }

    ARGBSurface ceiling(unsigned id) {
        if(id >= ceilings.size())
            return ARGBSurface(16, 16);
        return ceilings[id];
    }
}
