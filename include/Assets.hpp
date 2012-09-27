#ifndef ASSETS_HPP_INCLUDED

#include <vector>

#include "ARGBSurface.hpp"

namespace Assets {

    void init();
    ARGBSurface floor(unsigned id);
    ARGBSurface wall(unsigned id);
    ARGBSurface ceiling(unsigned id);
}

#define ASSETS_HPP_INCLUDED
#endif
