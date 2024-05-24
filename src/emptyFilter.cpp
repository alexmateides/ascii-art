#include "emptyFilter.h"

using PFilter = std::shared_ptr<CFilter>;

void CEmptyFilter::filter(SDL_Surface **surfacePtr) const {
    if (_nextFilter != nullptr){
        _nextFilter->filter(surfacePtr);
    }
}

PFilter CEmptyFilter::copy() const {
    auto newFilter = std::make_shared<CEmptyFilter>(*this);
    if(_nextFilter != nullptr){
        newFilter->_nextFilter = _nextFilter->copy();
    }
    else{
        newFilter->_nextFilter = nullptr;
    }
    return static_cast<PFilter>(newFilter);
}

PFilter CEmptyFilter::inverseCopy() const {
    return copy();
}