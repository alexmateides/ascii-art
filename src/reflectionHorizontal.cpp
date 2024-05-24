#include "reflectionHorizontal.h"

using PFilter = std::shared_ptr<CFilter>;

void CReflectionHorizontal::filter(SDL_Surface **surfacePtr) const {
    auto surface = *surfacePtr;
    uint32_t *pixels = static_cast<uint32_t *>(surface->pixels);
    for (size_t y = 0; y < surface->h; y++) {
        for (size_t x = 0; x < (surface->w)/2; x++) {
            uint32_t pixel = pixels[y * surface->w + x];
            pixels[y * surface->w + x] = pixels[((y+1) * surface->w) - x - 1];
            pixels[((y+1) * surface->w) - x - 1] = pixel;
        }
    }
    if (_nextFilter != nullptr){
        _nextFilter->filter(surfacePtr);
    }
}

PFilter CReflectionHorizontal::copy() const {
    auto newFilter = std::make_shared<CReflectionHorizontal>(*this);
    if(_nextFilter != nullptr){
        newFilter->_nextFilter = _nextFilter->copy();
    }
    else{
        newFilter->_nextFilter = nullptr;
    }
    return static_cast<PFilter>(newFilter);
}

PFilter CReflectionHorizontal::inverseCopy() const {
    return copy();
}