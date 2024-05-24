#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H

#pragma once
#include "filter.h"

/**=========================================
 *
 *                CBrightness
 *
 **=========================================
 * Changes brightness of the image
 * Either by flat value or percentage
 * =========================================*/
class CBrightness : public CFilter {
public:
    /**
     * Double       - initializes instance with PERCENTAGE value
     * percent > 1  - increase brightness
     * percent < 1  - decrease brightness
     * @param percent
     */
    CBrightness(double percent) : _percentAdjust(percent), _flatAdjust(0) {}

    /**
     * Int          - initializes instance with FLAT value
     * flat > 0     - increase brightness
     * flat < 0     - decrease brightness
     * @param flat
     */
    CBrightness(int flat) : _percentAdjust(-1), _flatAdjust(flat) {}

    /**
     * adjusts image by flat value
     * @param val - internal value
     * @return
     */
    uint8_t flatAdjust(uint8_t val) const;

    /**
     * adjusts image by percentage
     * @param val - internal value
     * @return
     */
    uint8_t percentAdjust(uint8_t val)const;

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
     * inverse method
     * @return  - shared_ptr to inverse
     */
    PFilter inverseCopy() const override;


private:
    /** variables holding adjustment values */
    double _percentAdjust;
    int _flatAdjust;
};

#endif
