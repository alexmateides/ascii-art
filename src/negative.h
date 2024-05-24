#ifndef SEMESTRALKAMAKE_NEGATIVE_H
#define SEMESTRALKAMAKE_NEGATIVE_H

#pragma once
#include "filter.h"

/**=========================================
 *
 *                CNegative
 *
 **=========================================
 * Applies negative filter to the image
 * =========================================*/
class CNegative: public CFilter{
public:
    /**
     * Applies filter
     * @param surface
     */
    void filter( SDL_Surface **surfacePtr) const override;

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

#endif //SEMESTRALKAMAKE_NEGATIVE_H
