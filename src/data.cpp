#include "data.h"

void CData::addImage(SDL_Surface *img) {
    _vecSurface.push_back(img);
}

void CData::addAlphabet(const std::vector<char> &vecAlphabet) {
    _vecAlphabet = vecAlphabet;
}

void CData::addEquation(const std::vector<double> &vecEquation) {
    _vecEquation = vecEquation;
}

void CData::addDefaultFilters(int angle, double resize, std::pair<int, double> brightness, size_t blur) {
    _defaultAngle = angle;
    _defaultResize = resize;
    _defaultBrightness = brightness;
    _defaultBlur = blur;
}

void CData::addSpecificFilters(std::shared_ptr<CFilter> globalFilter, std::vector<std::shared_ptr<CFilter>> vecFilter) {
    _globalFilter = globalFilter;
    _vecFilter = vecFilter;
}

void CData::addOutput(const std::string &outputPath) {
    _outputPath = outputPath;
}

void CData::toGreyscale(SDL_Surface *img) {
    uint32_t *pixelArr = static_cast<uint32_t *>(img->pixels);
    for (size_t y = 0; y < img->h; y++) {
        for (size_t x = 0; x < img->w; x++) {
            uint32_t pixel = pixelArr[y * img->w + x];
            //isolate color channels
            uint8_t red = pixel >> 16 & 0xff;
            uint8_t green = pixel >> 8 & 0xff;
            uint8_t blue = pixel & 0xff;

            //apply coefficients
            red = red * _vecEquation[0];
            blue = blue * _vecEquation[1];
            green = green * _vecEquation[2];
            //compute grayscale value
            uint8_t result = (red + green + blue);

            //combine channels set to grayscale
            pixel = result | (result << 8) | (result << 16) | (0xff << 24);
            pixelArr[y * img->w + x] = pixel;
        }
    }
}

void CData::allToGreyscale() {
    for (auto elem: _vecSurface) {
        this->toGreyscale(elem);
    }
}

void CData::allFilter() {
    for(size_t i = 0; i < _vecSurface.size(); i++){
        auto img = &_vecSurface[i];
        _globalFilter->filter(img);
        _vecFilter[i]->filter(img);
    }
}

void CData::printImg(std::ostream &oss, const SDL_Surface *img) {
    uint32_t *pixelArr = static_cast<uint32_t *>(img->pixels);
    double ratio = static_cast<double>(_vecAlphabet.size() - 1) / (3 * UINT8_MAX);
    for (size_t y = 0; y < img->h; y++) {
        if(y %2 == 0){
            for (size_t x = 0; x < img->w; x++) {
                uint32_t pixel = pixelArr[y * img->w + x];
                uint8_t alpha = pixel >> 24 & 0xff;
                uint8_t red = pixel >> 16 & 0xff;
                uint8_t green = pixel >> 8 & 0xff;
                uint8_t blue = pixel & 0xff;
                uint32_t result = floor(static_cast<double>(red + green + blue) * ratio);
                if (result >= _vecAlphabet.size()) {
                    result = _vecAlphabet.size() - 1;
                }
                //std::cout << result << "\n";
                oss << _vecAlphabet[result];
            }
            oss << "\n";
        }
    }
}

std::vector<uint32_t> CData::getIdx(const SDL_Surface *img){
    uint32_t *pixelArr = static_cast<uint32_t *>(img->pixels);
    std::vector<uint32_t > vecIdx;
    double ratio = static_cast<double>(_vecAlphabet.size() - 1) / (3 * UINT8_MAX);
    for (size_t y = 0; y < img->h; y++) {
        if (y % 2 == 0) {
            for (size_t x = 0; x < img->w; x++) {
                uint32_t pixel = pixelArr[y * img->w + x];
                uint8_t alpha = pixel >> 24 & 0xff;
                uint8_t red = pixel >> 16 & 0xff;
                uint8_t green = pixel >> 8 & 0xff;
                uint8_t blue = pixel & 0xff;
                uint32_t result = floor(static_cast<double>(red + green + blue) * ratio);
                if (result >= _vecAlphabet.size()) {
                    result = _vecAlphabet.size() - 1;
                }
                vecIdx.push_back(result);
            }
        }
    }
    return vecIdx;
}

std::vector<char> & CData::getAlphabet(){
    return _vecAlphabet;
}

std::shared_ptr<CFilter> CData::defaultRotate() const {
    return std::make_shared<CRotation>(_defaultAngle);
}

std::shared_ptr<CFilter> CData::defaultResize() const {
    return std::make_shared<CResize>(_defaultResize);
}

std::shared_ptr<CFilter> CData::defaultBrightness() const {
    if (_defaultBrightness.first != 0){
        return std::make_shared<CBrightness>(_defaultBrightness.first);
    }
    return std::make_shared<CBrightness>(_defaultBrightness.second);
}

void CData::freeSurfaces() {
    for (auto surface: _vecSurface){
        SDL_FreeSurface(surface);
    }
    _vecSurface.clear();
}