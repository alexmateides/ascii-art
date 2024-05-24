#ifndef ATLAS_H
#define ATLAS_H

#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include <sstream>


/**=============================
 *
 *           CAtlas
 *
 *==============================
 * Glyph atlas that speeds up text rendering
 *==============================*/
class CAtlas{
public:
    friend class CApp;
    CAtlas() = default;

    /**
     * Initializes the glyph atlas by rendering all characters in vecChar
     * the rendered characters are stored as textures in _vecTexture
     * @param vecChar
     * @param font
     * @param renderer
     */
    CAtlas(const std::vector<char> & vecChar, TTF_Font * font, SDL_Renderer * renderer);

    ~CAtlas(){
        for (auto elem: _vecTexture){
            //SDL_DestroyTexture(elem);
        }
    }

    /**
     * Renders image represented by vector of corresponding indexes vecIdx
     * @param window
     * @param renderer
     * @param vecIdx
     * @param width     - width of original image
     * @param height    - height of original image
     * @return          - texture containing the rendered image
     */
    SDL_Texture * renderString(SDL_Window * window ,SDL_Renderer * renderer, std::vector<uint32_t> & vecIdx, int width, int height);

    /**
     * Destroys all rendered glyph textures, invalidates the atlas
     */
    void freeGlyphs();


private:
    int _fontWidth;
    int _fontHeight;
    std::vector<SDL_Texture *> _vecTexture;
};

#endif