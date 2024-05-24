#include "negative.h"

using PFilter = std::shared_ptr<CFilter>;

void CNegative::filter( SDL_Surface **surfacePtr) const {
    auto surface = *surfacePtr;
    uint32_t *pixels = static_cast<uint32_t *>(surface->pixels);

    /** each pixel gets turned into negative value */
    for (size_t y = 0; y < surface->h; y++) {
        for (size_t x = 0; x < surface->w; x++) {
            uint32_t pixel = pixels[y * surface->w + x];
            uint8_t red = UINT8_MAX - (pixel >> 16 & 0xFF);
            uint8_t green = UINT8_MAX - (pixel >> 8 & 0xFF);
            uint8_t blue = UINT8_MAX - (pixel & 0xFF);
            //combine channels
            pixels[y * surface->w + x] = (red << 16 | green << 8 | blue);
        }
    }
    if (_nextFilter != nullptr){
        _nextFilter->filter(surfacePtr);
    }
}

PFilter CNegative::copy() const {
    auto newFilter = std::make_shared<CNegative>(*this);
    if(_nextFilter != nullptr){
        newFilter->_nextFilter = _nextFilter->copy();
    }
    else{
        newFilter->_nextFilter = nullptr;
    }
    return static_cast<PFilter>(newFilter);
}

PFilter CNegative::inverseCopy() const {
    return copy();
}