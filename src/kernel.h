#ifndef SEMESTRALKAMAKE_KERNEL_H

#pragma once
#include <initializer_list>
#include <vector>
#include "filter.h"


/**=========================================
 *
 *                CKernel
 *
 **=========================================
 * Convolutional filter
 * =========================================*/
class CKernel: public CFilter{
public:
    /**
     * Directly initializes kernel matrix
     * @param initMatrix
     */
    CKernel(std::initializer_list<std::initializer_list<double>> initMatrix);

    /**
     * Initializes kernel as n*n matrix of 1s
     * @param blurStrength - size of half kernel (1 -> 3x3, 2 -> 5x5)
     */
    CKernel(size_t blurStrength);

    /**
     * Applies convolution to the given pixel
     * @param pixels    - surface pixel array
     * @param x         - x coordinate
     * @param y         - y coordinate
     * @param w         - surface width
     * @param h         - surface height
     * @return          - computed value at [x,y]
     */
    uint32_t applyConvolution(uint32_t *pixels, size_t x, size_t y, size_t w, size_t h) const;

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
     * inverse method - due to convolution nature, can't exist
     * copies the filter instead
     * @return  - shared_ptr to copy
     */
    PFilter inverseCopy() const override;
private:
    std::vector<std::vector<double>> _matrix;
};

#define SEMESTRALKAMAKE_KERNEL_H

#endif //SEMESTRALKAMAKE_KERNEL_H
