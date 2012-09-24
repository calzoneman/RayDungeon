#ifndef LEVEL_HPP_INCLUDED

#include "IntDefs.hpp"

class Level {
    private:
        unsigned _width, _height;
        u16 *data;
    public:
        Level(unsigned width, unsigned height);
        Level();

        unsigned width() const;
        unsigned height() const;

        enum Layer {
            Floor = 0,
            Wall = 1,
            Ceiling = 2
        };
        u16 operator()(unsigned x, unsigned y, Layer layer);
        u16 get(unsigned x, unsigned y, Layer layer);
        bool set(unsigned x, unsigned y, Layer layer, u16 val);
};

#define LEVEL_HPP_INCLUDED
#endif
