#include "resize.h"
using PFilter = std::shared_ptr<CFilter>;

void CResize::filter(SDL_Surface **surfacePtr) const {
    auto surface = * surfacePtr;
    auto newSurface = rotozoomSurface(surface, 0, _resizeFactor, 0);
    SDL_FreeSurface(surface);
    *surfacePtr = newSurface;
    if (_nextFilter != nullptr){
        _nextFilter->filter(surfacePtr);
    }
}

PFilter CResize::copy() const {
    auto newFilter = std::make_shared<CResize>(*this);
    if(_nextFilter != nullptr){
        newFilter->_nextFilter = _nextFilter->copy();
    }
    else{
        newFilter->_nextFilter = nullptr;
    }
    return static_cast<PFilter>(newFilter);
}

PFilter CResize::inverseCopy() const {
    auto newFilter = std::make_shared<CResize>(*this);
    newFilter->_resizeFactor = 1/_resizeFactor;
    if(_nextFilter != nullptr){
        newFilter->_nextFilter = _nextFilter->inverseCopy();
    }
    else{
        newFilter->_nextFilter = nullptr;
    }
    return static_cast<PFilter>(newFilter);
}
