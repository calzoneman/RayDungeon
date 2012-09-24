#include <cstddef>

#include "Level.hpp"
#include "IntDefs.hpp"

Level::Level() {
    this->width = 0;
    this->height = 0;
    this->data = NULL;
}

Level::Level(unsigned width, unsigned height) {
    this->width = width;
    this->height = height;
    this->data = new u16[width * height * 2];
    for(unsigned i = 0; i < width * height * 2; i++) {
        this->data[i] = 0;
    }
}

u16 Level::operator()(unsigned x, unsigned y, Layer layer) {
    return this->get(x, y, layer);
}

u16 Level::get(unsigned x, unsigned y, Layer layer) {
    int iLayer = static_cast<int>(layer);
    if(x < 0 || y < 0 || iLayer < 0 || x >= this->width || y >= this->height || iLayer > 2) {
        return static_cast<u16>(-1);
    }
    return this->data[(iLayer * this->height + y) * this->width + x];
}

bool Level::set(unsigned x, unsigned y, Layer layer, u16 val) {
    int iLayer = static_cast<int>(layer);
    if(x < 0 || y < 0 || iLayer < 0 || x >= this->width || y >= this->height || iLayer > 2) {
        return false;
    }
    this->data[(iLayer * this->height + y) * this->width + x] = val;
    return true;
}
