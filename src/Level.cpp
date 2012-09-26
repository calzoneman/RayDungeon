#include <cstddef>

#include "Level.hpp"
#include "IntDefs.hpp"

Level::Level() {
    this->_width = 0;
    this->_height = 0;
    this->data = NULL;
}

Level::Level(unsigned width, unsigned height) {
    this->_width = width;
    this->_height = height;
    this->data = new u16[width * height * 2];
    for(unsigned i = 0; i < width * height * 2; i++) {
        if(i/width == 0 || i%width == 0 || i/width == height || i%width == height-1)
            this->data[i] = 1;
        else
            this->data[i] = 0;
    }
}

unsigned Level::width() const {
    return this->_width;
}

unsigned Level::height() const {
    return this->_height;
}

u16 Level::operator()(unsigned x, unsigned y, Layer layer) {
    return this->get(x, y, layer);
}

u16 Level::get(unsigned x, unsigned y, Layer layer) {
    int iLayer = static_cast<int>(layer);
    if(x < 0 || y < 0 || iLayer < 0 || x >= this->_width || y >= this->_height || iLayer > 2) {
        return static_cast<u16>(-1);
    }
    return this->data[(iLayer * this->_height + y) * this->_width + x];
}

bool Level::set(unsigned x, unsigned y, Layer layer, u16 val) {
    int iLayer = static_cast<int>(layer);
    if(x < 0 || y < 0 || iLayer < 0 || x >= this->_width || y >= this->_height || iLayer > 2) {
        return false;
    }
    this->data[(iLayer * this->_height + y) * this->_width + x] = val;
    return true;
}
