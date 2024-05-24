#ifndef SEMESTRALKAMAKE_DATA_H
#define SEMESTRALKAMAKE_DATA_H


#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "filter.h"
#include "rotation.h"
#include "brightness.h"
#include "resize.h"

/**=========================================
 *
 *                CDATA
 *
 **=========================================
 * Handles data loading and pre-processing
 * =========================================*/
class CData {
public:
    friend class CApp;
    /**
     * Adds img to _vecSurface
     * @param img
     */
    void addImage(SDL_Surface *img);

    /**
     * Adds ASCII-character vector
     * lowest idx -> darkest character
     * @param vecAlphabet
     */
    void addAlphabet(const std::vector<char> &vecAlphabet);

    /**
     * Adds equation that contains grayscale ratio
     * Should contain three positive numbers that add up to 1
     * Describes ratio at which are RGB channels converted to grayscale
     * @param vecEquation
     */
    void addEquation(const std::vector<double> &vecEquation);

    /**
     * Adds values of default filters
     * @param angle         - default rotation angle
     * @param zoom          - default zoom
     * @param brightness    - default brightness change
     */
    void addDefaultFilters(int angle, double resize, std::pair<int, double> brightness, size_t blur);

    /**
     * Adds values of specific filters, that are applied during preprocessing
     * @param globalFilter  - is applied to all images
     * @param filterVec     - contains filter for each image
     */
    void addSpecificFilters(std::shared_ptr<CFilter> globalFilter, std::vector<std::shared_ptr<CFilter>> vecFilter);

    /**
     * Converts img to greyscale according to _vecEquation
     * @param img
     */
    void toGreyscale(SDL_Surface *img);

    /**
     * Adds path to output file, where images will be printed
     * @param outputPath    - path to the file
     */
    void addOutput(const std::string & outputPath);

    /**
     * Converts all images to greyscale
     */
    void allToGreyscale();

    /**
     * Applies all preprocessing filters
     */
    void allFilter();

    /**
     * Outputs image to output stream in ASCII-art form
     * Uses 2:1 vertical correction (skips each second row)
     * because fonts tend to be of similar ratio and the image
     * looks more natural
     * @param oss
     * @param img
     */
    void printImg(std::ostream &oss, const SDL_Surface *img);

    /**
     * Calculates corresponding ASCII character for each pixel in img
     * the value is index in _vecAlphabet
     * @param img
     * @return vector of indexes in _vecAlphabet
     */
    std::vector<uint32_t > getIdx(const SDL_Surface *img);

    std::vector<char> & getAlphabet();

    /**
     * Creates default filter from data
     * @return default rotation filter
     */
    std::shared_ptr<CFilter> defaultRotate() const;

    /**
     * Create default filter from data
     * @return default resize filter
     */
    std::shared_ptr<CFilter> defaultResize() const;

    /**
     * Create default filter from dta
     * @return default brightness filter
     */
    std::shared_ptr<CFilter> defaultBrightness() const;

    /**
     * Destroys all surfaces and resizes _vecSurface
     */
    void freeSurfaces();


private:
    /** All images */
    std::vector<SDL_Surface *> _vecSurface;

    /** Vector containing default filters for preprocessing */
    std::vector<std::shared_ptr<CFilter>> _vecFilter;

    /** Filter for all images */
    std::shared_ptr<CFilter> _globalFilter;

    /** ASCII characters, [0] is darkest */
    std::vector<char> _vecAlphabet;

    /** Three doubles representing RGB conversion ratio */
    std::vector<double> _vecEquation;

    /** path to output file */
    std::string _outputPath;

    /** Default filter values */
    int _defaultAngle;
    double _defaultResize;
    std::pair<int, double> _defaultBrightness;
    size_t _defaultBlur;
};

#endif //SEMESTRALKAMAKE_DATA_H
