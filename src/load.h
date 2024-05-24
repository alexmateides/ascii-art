#ifndef SEMESTRALKAMAKE_LOAD_H
#define SEMESTRALKAMAKE_LOAD_H

#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "data.h"
#include "../libs/json.hpp"
#include "filter.h"
#include "rotation.h"
#include "resize.h"
#include "reflectionHorizontal.h"
#include "reflectionVertical.h"
#include "negative.h"
#include "brightness.h"
#include "emptyFilter.h"
#include "kernel.h"

/**=========================================
 *
 *                CLoader
 *
 **=========================================
 * Parses and handles the contents of config file
 * =========================================*/
class CLoader{
public:
    /**
     * @param filePath - path to config
     */
    CLoader(const std::string & filePath): _filePath(filePath){}

    /**
     * Loads CData instance from config
     * @param data  - CData instance
     * @return      - true on success, false on failure
     */
    bool parse(CData & data);

    /**
     * Loads image data from config
     * @param vecImagePath  - vector of paths to images
     * @param data          - CData instance, stores result
     * @return              - true on success, false on failure
     */
    bool parseImages(const std::string & folder, const std::vector<std::string>& vecImagePath, CData & data);

    /**
     * Loads equation from config
     * @param vecEquation   - equation from config
     * @param data          - CData instance, stores result
     * @return              - true on success, false on failure
     */
    bool parseEquation(const std::vector<double>& vecEquation, CData & data);

    /**
     * Loads ASCII alphabet from config
     * @param strAlphabet   - alphabet string from config
     * @param data          - CData instance, stores result
     * @return              - true on success, false on failure
     */
    bool parseAlphabet(const std::string& strAlphabet, CData & data);

    /**
     * Sets values of default filter
     * @param filters       - json block of filters config (NLOHMANN)
     * @param data          - CData instance, stores result
     * @return              - true on success, false on failure
     */
    bool parseDefaultFilters(nlohmann::basic_json<> filters, CData & data);

    /**
     * Sets values of preprocessing filters
     * @param imgNames      - vector of paths to images
     * @param filters       - json block of filters config (NLOHMANN)
     * @param data          - CData instance, stores result
     * @return              - true on success, false on failure
     */
    bool parseFilters(const std::vector<std::string> & imgNames, const nlohmann::basic_json<>& filters, CData &data);

    /**
     * Creates appropriate CFilter instance from json format
     * @param filter        - json block of single filter config (NLOHMANN)
     * @return              - true on success, false on failure
     */
    std::shared_ptr<CFilter> getFilter(const nlohmann::basic_json<> & filter);

    /**
     * Sets output file path
     * @param filePath      - path to output file
     * @param data          - CData instance, stores result
     * @return              - true on success, false on failure
     */
    bool parseOutputPath(const std::string& filePath, CData & data);

private:
    std::string _filePath;
};

#endif //SEMESTRALKAMAKE_LOAD_H
