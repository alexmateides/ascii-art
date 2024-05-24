#include "reflectionVertical.h"

using PFilter = std::shared_ptr<CFilter>;

void CReflectionVertical::filter(SDL_Surface **surfacePtr) const {
    auto surface = *surfacePtr;
    uint32_t *pixels = static_cast<uint32_t *>(surface->pixels);
    for (size_t y = 0; y < surface->h/2; y++) {
        for (size_t x = 0; x < surface->w; x++) {
            uint32_t pixel = pixels[y * surface->w + x];
            pixels[y * surface->w + x] = pixels[(surface->h - y - 1) * surface->w + x];
            pixels[(surface->h - y - 1) * surface->w + x] = pixel;
        }
    }
    if (_nextFilter != nullptr){
        _nextFilter->filter(surfacePtr);
    }
}

PFilter CReflectionVertical::copy() const {
    auto newFilter = std::make_shared<CReflectionVertical>(*this);
    if(_nextFilter != nullptr){
        newFilter->_nextFilter = _nextFilter->copy();
    }
    else{
        newFilter->_nextFilter = nullptr;
    }
    return static_cast<PFilter>(newFilter);
}

PFilter CReflectionVertical::inverseCopy() const {
    return copy();
}
