#ifndef SEMESTRALKAMAKE_FILTER_H
#define SEMESTRALKAMAKE_FILTER_H


#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL2_rotozoom.h>
#include <vector>
#include <memory>
#include <algorithm>

/**=========================================
 *
 *                CFilter
 *
 **=========================================
 * Modifies images by different means
 * Abstract parent class
 * =========================================*/
class CFilter {
public:
    using PFilter = std::shared_ptr<CFilter>;
    /**
     * Virtual method that applies chosen filter to SDL_Surface image
     * applies _nextFilter if possible
     * @param surface
     */
    virtual void filter(SDL_Surface **surfacePtr) const = 0;

    /**
     * Creates a deep copy of the filter
     * @return - pointer to the copy
     */
    virtual PFilter copy() const = 0;


    /**
     * Creates a new filter, that is inverse to this
     * @return - pointer to new filter
     */
    virtual PFilter inverseCopy() const = 0;

    /**
     * Adds filter at the end of chain
     * @param newFilter filter to be added to chain
     */
    void addChain(PFilter newFilter);

    /**
     * Takes a number of CFilter instances and chains them together
     * @param vecInit   - vector of shared_ptr to be chained
     * @return          - pointer to first instance of the resulting chain
     *                    filtering by result applies the entire chain
     */
    static PFilter mergeFilters(std::vector<PFilter>  vecInit);

protected:
    /** pointer to next instance in chain, nullptr if this is last in chain*/
    PFilter _nextFilter;
};

#endif //SEMESTRALKAMAKE_FILTER_H
