#include "atlas.h"

CAtlas::CAtlas(const std::vector<char> &vecChar, TTF_Font * font, SDL_Renderer * renderer) {
    std::string tmp;
    SDL_Color black = {0,0,0,0};
    SDL_Rect viewPort;
    viewPort.x = 0;
    viewPort.y = 0;
    TTF_SizeText(font, "|", &_fontWidth, &_fontHeight);
    viewPort.w = _fontWidth;
    viewPort.h = _fontHeight;
    for (auto character: vecChar){
        tmp.push_back(character);
        SDL_Surface * surface = TTF_RenderText_Blended(font, tmp.c_str(), black);
        SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        _vecTexture.push_back(texture);
        tmp.resize(0);
    }
}

SDL_Texture * CAtlas::renderString(SDL_Window * window ,SDL_Renderer * renderer, std::vector<uint32_t > & vecIdx, int width, int height) {
    SDL_Rect viewPort;
    viewPort.x = 0;
    viewPort.y = 0;
    viewPort.w = _fontWidth;
    viewPort.h = _fontHeight;
    SDL_SetWindowSize(window,width*_fontWidth, (height/2)  * (_fontHeight-1));
    size_t idx = 0;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer);
    int w,h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, width*_fontWidth, height  * (_fontHeight-1));
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderClear(renderer);
    for (int y = 0; y < height; y++) {
        viewPort.y = y * (_fontHeight-1);
        viewPort.x = 0;
        for (int x = 0; x < width; x++) {
            if (idx >= vecIdx.size()) {
                idx = vecIdx.size() - 1;
            }
            SDL_RenderCopy(renderer, _vecTexture[vecIdx[idx]], NULL, &viewPort);

            viewPort.x += _fontWidth;
            idx++;
        }
    }

    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    return texture;
}

void CAtlas::freeGlyphs() {
    for (auto texture: _vecTexture){
        SDL_DestroyTexture(texture);
    }
}