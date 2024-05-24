#ifndef SEMESTRALKAMAKE_EMPTYFILTER_H
#define SEMESTRALKAMAKE_EMPTYFILTER_H

#pragma once
#include "filter.h"



/**=========================================
 *
 *                CEmptyFilter
 *
 **=========================================
 * Empty filter, used as head for chains
 * Doesn't do anything by itself
 * =========================================*/
class CEmptyFilter: public CFilter{
    /**
     * Applies filter
     * @param surface
     */
    void filter(SDL_Surface **surfacePtr) const override;

    /**
     * Deep copy method
     * @return - shared_ptr to new instance
     */
    PFilter copy() const override;

    /**
     * inverse method - applies the same filter again
     * @return  - shared_ptr to inverse
     */
    PFilter inverseCopy() const override;
};

#endif //SEMESTRALKAMAKE_EMPTYFILTER_H
