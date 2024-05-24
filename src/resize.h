#ifndef SEMESTRALKAMAKE_RESIZE_H
#define SEMESTRALKAMAKE_RESIZE_H

#pragma once
#include "filter.h"


/**=========================================
 *
 *          CResize
 *
 **=========================================
 * Resizes image by a given factor
 * =========================================*/
class CResize: public CFilter{
public:
    CResize(double factor): _resizeFactor(factor){}

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
private:
    /**
     * _resizeFactor > 1    ->  Image upscale
     * _resizeFactor < 1    ->  Image downscale
     */
    double _resizeFactor;
};

#endif //SEMESTRALKAMAKE_RESIZE_H
