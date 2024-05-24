#include "load.h"

bool CLoader::parseEquation(const std::vector<double>& vecEquation, CData & data) {
    try {
        std::vector<double> vecResult;
        for (const auto &coefficient: vecEquation) {
            vecResult.push_back(coefficient);
        }
        if (vecResult.size() != 3) {
            throw std::invalid_argument("INVALID CONFIG - WRONG NUMBER OF EQUATION COEFFICIENTS");
        }
    }
    catch (...){
        return false;
    }
    data.addEquation(vecEquation);
    return true;
}

bool CLoader::parseImages(const std::string & folder, const std::vector<std::string>& vecImagePath, CData &data) {
    try{
        for (const auto &path: vecImagePath) {
            std::string pathStr = folder + path;
            SDL_Surface *newSurface = IMG_Load(pathStr.c_str());
            auto formatSurface =  SDL_ConvertSurfaceFormat(newSurface, SDL_PIXELFORMAT_ARGB8888,0);
            SDL_FreeSurface(newSurface);
            if (newSurface == NULL){
                throw std::invalid_argument("INVALID IMAGE - DIDNT LOAD");
            }
            data.addImage(formatSurface);
        }
        if (vecImagePath.empty()) {
            throw std::invalid_argument("INVALID CONFIG - NO IMAGES");
        }
    }
    catch (...){
        return false;
    }
    return true;
}

bool CLoader::parseAlphabet(const std::string& strAlphabet, CData &data) {
    try {
        std::vector<char> vecAlphabet;
        for (auto character: strAlphabet) {
            vecAlphabet.push_back(character);
        }
        if (vecAlphabet.empty()) {
            throw std::invalid_argument("INVALID CONFIG - EMPTY ALPHABET");
        }
        data.addAlphabet(vecAlphabet);
    }
    catch(...){
        return false;
    }
    return true;
}

bool CLoader::parseDefaultFilters(nlohmann::basic_json<> filters, CData & data) {
    std::pair<int, double> valBrightness = std::make_pair(0,-1);
    int angle = 0;
    double resize = 1;
    int blur = 0;

    try{
        if(filters.contains("brightness")){
            auto brightness = filters["brightness"];
            if (brightness[0] == "A"){
                valBrightness.first = brightness[1];
            }
            else if(brightness[0] == "R"){
                valBrightness.second = brightness[1];
            }
        }
        if(filters.contains("rotation")){
            angle = filters["rotation"];
        }
        if(filters.contains("resize")){
            resize = filters["resize"];
            if (resize < 0){
                throw(std::invalid_argument("INVALID CONFIG - NEGATIVE SCALING"));
            }
        }

        if(filters.contains("blur")){
            blur = filters["blur"];
            if (blur < 0){
                throw(std::invalid_argument("INVALID CONFIG - NEGATIVE BLUR"));
            }
        }

        data.addDefaultFilters(angle, resize, valBrightness, blur);

    }
    catch (...){
        return false;
    }
    return true;
}

bool CLoader::parseFilters(const std::vector<std::string> & imgNames, const nlohmann::basic_json<>& filters, CData &data) {
    try{
        auto globalFilter = std::make_shared<CEmptyFilter>();
        if (filters.contains("all")){
            for (const auto& filter: filters["all"]){
                std::shared_ptr<CFilter> filterPtr = getFilter(filter);
                globalFilter->addChain(filterPtr);
            }
        }
        std::vector<std::shared_ptr<CFilter>> vecFilter;
        for (const auto& name: imgNames){
            auto head = std::make_shared<CEmptyFilter>();
            if(filters.contains(name)){
                for (const auto& filter: filters[name]){
                    std::shared_ptr<CFilter> filterPtr = getFilter(filter);
                    head->addChain(filterPtr);
                }
            }
            vecFilter.push_back(head);
        }

        data.addSpecificFilters(globalFilter, vecFilter);
    }
    catch (...){
        return false;
    }
    return true;
}

std::shared_ptr<CFilter> CLoader::getFilter(const nlohmann::basic_json<> &filter) {
    try{
        std::string filterName = filter["filterName"];
        if (filterName == "brightness"){
            std::string type = filter["type"];
            if (type == "A"){
                int val = filter["value"];
                return std::make_shared<CBrightness>(val);
            }
            else if (type == "R"){
                double val = filter["value"];
                return std::make_shared<CBrightness>(val);
            }
            else{
                throw std::invalid_argument("INVALID CONFIG - WRONG BRIGHTNESS TYPE");
            }
        }
        else if(filterName == "negative"){
            return std::make_shared<CNegative>();
        }
        else if(filterName == "resize"){
            double factor = filter["factor"];
            if (factor <= 0){
                throw std::invalid_argument("INVALID CONFIG - NEGATIVE SIZE");
            }
            return std::make_shared<CResize>(factor);
        }
        else if(filterName == "reflectionHorizontal"){
            return std::make_shared<CReflectionHorizontal>();
        }
        else if(filterName == "reflectionVertical"){
            return std::make_shared<CReflectionVertical>();
        }
        else if(filterName == "rotation"){
            int angle = filter["angle"];
            return std::make_shared<CRotation>(angle);
        }
        else if(filterName == "blur"){
            int strength = filter["strength"];
            return std::make_shared<CKernel>(strength);
        }
        else{
            return nullptr;
        }
    }
    catch (...){
        return nullptr;
    }

}

bool CLoader::parseOutputPath(const std::string& filePath, CData & data) {
    try{
        data.addOutput(filePath);
    }
    catch(...){
        return false;
    }

    return true;
}

bool CLoader::parse(CData & data) {
    try{
        std::ifstream fss(_filePath);
        std::stringstream tmp;
        tmp << fss.rdbuf();
        auto parsedJson = nlohmann::json::parse(tmp.str());
        if (
            !parseImages(parsedJson["img_folder"],parsedJson["img_paths"], data)                         ||
            !parseEquation(parsedJson["equation"], data)                        ||
            !parseAlphabet(parsedJson["alphabet"], data)                        ||
            !parseDefaultFilters(parsedJson["defaultFilters"], data)            ||
            !parseFilters(parsedJson["img_paths"], parsedJson["filters"], data) ||
            !parseOutputPath(parsedJson["output_path"], data)
        ){
            throw std::invalid_argument("INVALID CONFIG");
        }
        data.allToGreyscale();
        data.allFilter();
    }
    catch (...){
        /** frees allocated on error */
        data.freeSurfaces();
        return false;
    }
    return true;
}

