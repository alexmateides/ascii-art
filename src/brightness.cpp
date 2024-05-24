#include "brightness.h"

using PFilter = std::shared_ptr<CFilter>;

uint8_t CBrightness::flatAdjust(uint8_t val) const{
    int result = val;
    if (result < 0){
        return 0;
    }
    if (result > 255){
        return 255;
    }
    return static_cast<uint8_t>(result);
}

uint8_t CBrightness::percentAdjust(uint8_t val)const{
    if (val < 7){
        val += 1;
    }
    int result = val * _percentAdjust;
    if (result >= UINT8_MAX ){
        return UINT8_MAX;
    }
    return static_cast<uint8_t>(result);
}

void CBrightness::filter(SDL_Surface **surfacePtr) const {
    auto surface = *surfacePtr;
    uint32_t *pixels = static_cast<uint32_t *>(surface->pixels);

    /** change brightness of each pixel */
    for (size_t y = 0; y < surface->h; y++) {
        for (size_t x = 0; x < surface->w; x++) {
            uint32_t pixel = pixels[y * surface->w + x];
            if (_flatAdjust != 0){
                uint8_t red = flatAdjust(pixel >> 16 & 0xFF);
                uint8_t green = flatAdjust(pixel >> 8 & 0xFF);
                uint8_t blue = flatAdjust(pixel & 0xFF);
                /** combine channels */
                pixels[y * surface->w + x] = (red << 16 | green << 8 | blue);
            }
            if (_percentAdjust >= 0) {
                uint8_t red = percentAdjust(pixel >> 16 & 0xFF);
                uint8_t green = percentAdjust(pixel >> 8 & 0xFF);
                uint8_t blue = percentAdjust(pixel & 0xFF);
                pixels[y * surface->w + x] = (red << 16 | green << 8 | blue);
            }
        }
    }
    if (_nextFilter != nullptr){
        _nextFilter->filter(surfacePtr);
    }
}

PFilter CBrightness::copy() const {
    auto newFilter = std::make_shared<CBrightness>(*this);
    if(_nextFilter != nullptr){
        newFilter->_nextFilter = _nextFilter->copy();
    }
    else{
        newFilter->_nextFilter = nullptr;
    }
    return static_cast<PFilter>(newFilter);
}

PFilter CBrightness::inverseCopy() const {
    auto newFilter = std::make_shared<CBrightness>(*this);
    newFilter->_percentAdjust = 1/newFilter->_percentAdjust;
    newFilter->_flatAdjust *= -1;
    if(_nextFilter != nullptr){
        newFilter->_nextFilter = _nextFilter->inverseCopy();
    }
    else{
        newFilter->_nextFilter = nullptr;
    }
    return static_cast<PFilter>(newFilter);
}
