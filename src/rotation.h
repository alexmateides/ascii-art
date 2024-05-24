#ifndef SEMESTRALKAMAKE_ROTATION_H
#define SEMESTRALKAMAKE_ROTATION_H

#pragma once
#include "filter.h"


/**=========================================
 *
 *          CRotation
 *
 **=========================================
 * Rotates image around center
 * =========================================*/
class CRotation: public CFilter{
public:
    CRotation(int angle): _angle(angle){}

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
    /** rotation angle in degrees
     * positive - counterclockwise
     * negative - clockwise         **/
    int _angle;
};


#endif //SEMESTRALKAMAKE_ROTATION_H
