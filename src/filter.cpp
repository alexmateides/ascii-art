#include "filter.h"
#include <utility>

using PFilter = std::shared_ptr<CFilter>;

void CFilter::addChain(CFilter::PFilter newFilter) {
    if (_nextFilter == nullptr){
        _nextFilter = std::move(newFilter);
    }
    else{
        _nextFilter->addChain(newFilter);
    }
}

PFilter CFilter::mergeFilters(std::vector<PFilter> vecInit) {
    std::reverse(vecInit.begin(), vecInit.end());
    PFilter previous = nullptr;
    PFilter result = nullptr;
    for (const auto& ptr: vecInit){
        PFilter newPtr = ptr->copy();
        newPtr->_nextFilter = previous;
        previous = newPtr;
        result = newPtr;
    }

    return result;
}

