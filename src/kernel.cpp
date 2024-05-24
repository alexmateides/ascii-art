#include "kernel.h"
using PFilter = std::shared_ptr<CFilter>;

void capSize(uint32_t * num){
    if (*num > 255){
        *num = 255;
    }
}

CKernel::CKernel(std::initializer_list<std::initializer_list<double>> initMatrix) {
    for (auto elem: initMatrix) {
        _matrix.emplace_back(std::vector<double>(elem));
    }
}

CKernel::CKernel(size_t blurStrength) {
    blurStrength *= 2;
    blurStrength++;
    for (size_t i = 0; i < blurStrength; i++){
        std::vector<double> tmp;
        tmp.resize(blurStrength);
        std::fill(tmp.begin(), tmp.end(), 1);
        _matrix.emplace_back(tmp);
    }
}

uint32_t CKernel::applyConvolution(uint32_t *pixels, size_t x, size_t y, size_t w, size_t h) const{
    size_t halfLen = _matrix.size() / 2;
    x -= halfLen;
    y -= halfLen;
    uint32_t redTotal = 0, blueTotal = 0, greenTotal = 0;

    size_t count = 0;
    /** iterate through pixels and apply kernel */
    for (size_t i = 0; i < _matrix.size(); i++){
        for(size_t j = 0; j < _matrix.size(); j++){
            if (x + j > 0 && x + j < w && y + i > 0 && y + i < h){
                count++;
                uint32_t pixel = pixels[(y + i) * w + (x + j)];

                /** isolate channels */
                uint32_t alpha = pixel >> 24 & 0xff;
                uint32_t red = pixel >> 16 & 0xff;
                uint32_t green = pixel >> 8 & 0xff;
                uint32_t blue = pixel & 0xff;

                /** apply kernel multiplier */
                double multiplier = _matrix[i][j];
                red *= multiplier; green *= multiplier; blue *= multiplier;
                capSize(&red); capSize(&green); capSize(&blue);
                redTotal += red; blueTotal += blue, greenTotal += green;
            }
        }
    }

    /** normalize */
    redTotal /= count; blueTotal /= count; greenTotal /= count;

    /** connect channels */
    return (redTotal << 16 | greenTotal << 8 | blueTotal);
}


void CKernel::filter(SDL_Surface **surfacePtr) const {
    auto surface = *surfacePtr;
    uint32_t *pixels = static_cast<uint32_t *>(surface->pixels);

    /** iterate through pixels and apply kernel */
    for (size_t y = 0; y < surface->h; y++) {
        for (size_t x = 0; x < surface->w; x++) {
            pixels[y * surface->w + x] = applyConvolution(pixels, x, y, surface->w, surface->h);
        }
    }
    if (_nextFilter != nullptr){
        _nextFilter->filter(surfacePtr);
    }
}

PFilter CKernel::copy() const {
    auto newFilter = std::make_shared<CKernel>(*this);
    if(_nextFilter != nullptr){
        newFilter->_nextFilter = _nextFilter->copy();
    }
    else{
        newFilter->_nextFilter = nullptr;
    }
    return static_cast<PFilter>(newFilter);
}

PFilter CKernel::inverseCopy() const {
    return copy();
}
