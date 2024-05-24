#include "rotation.h"
using PFilter = std::shared_ptr<CFilter>;

void CRotation::filter(SDL_Surface **surfacePtr) const {
    auto surface = *surfacePtr;
    auto newSurface = rotozoomSurface(surface, _angle, 1, 0);
    if(_angle%90 != 0){
        size_t width = surface->w, height = surface->h;
        auto newSurface2 = zoomSurface(newSurface,(double)width/newSurface->w,(double)height/newSurface->h,0);
        SDL_FreeSurface(newSurface);
        SDL_FreeSurface(surface);
        *surfacePtr = newSurface2;
        return;
    }
    *surfacePtr = newSurface;
    SDL_FreeSurface(surface);
    if (_nextFilter != nullptr){
        _nextFilter->filter(surfacePtr);
    }
}

PFilter CRotation::copy() const {
    auto newFilter = std::make_shared<CRotation>(*this);
    if(_nextFilter != nullptr){
        newFilter->_nextFilter = _nextFilter->copy();
    }
    else{
        newFilter->_nextFilter = nullptr;
    }
    return static_cast<PFilter>(newFilter);
}

PFilter CRotation::inverseCopy() const {
    auto newFilter = std::make_shared<CRotation>(*this);
    newFilter->_angle = -_angle;
    if(_nextFilter != nullptr){
        newFilter->_nextFilter = _nextFilter->inverseCopy();
    }
    else{
        newFilter->_nextFilter = nullptr;
    }
    return static_cast<PFilter>(newFilter);
}

