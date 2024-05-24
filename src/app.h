#ifndef SEMESTRALKAMAKE_APP_H
#define SEMESTRALKAMAKE_APP_H

#pragma once
#include <fstream>
#include "atlas.h"
#include "data.h"
#include "negative.h"
#include "reflectionHorizontal.h"
#include "reflectionVertical.h"
#include "brightness.h"
#include "rotation.h"
#include "resize.h"
#include "kernel.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/** file path to ttf font file that is used for rendering */
constexpr const char * FONT_PATH = "./assets/fonts/gnu_unifont.ttf";

/** size of font that will be rendered in GUI */
constexpr size_t FONTSIZE_DISPLAY = 4;

/** delay between presentation slides in milliseconds */
constexpr size_t PRESENTATION_DELAY = 2000;

/**=============================
 *
 *           CApp
 *
 *==============================
 * Handles GUI and image rendering
 *==============================*/
class CApp {
public:
    CApp(CData & data);

    ~CApp();

    /** Handles events and application flow*/
    void mainLoop();


    /** Renders ASCII-art of currently displayed image */
    void processImg();

    /** Pre-renders ASCII-art of all images
     *  used as cache for faster browsing */
    void processAll();

    /**
     * Changes to and displays image at images[_idx]
     * @return current image
     */
    void switchImage();


    /** Re-renders both windows and their displayed images*/
    void renderAll();

    /** Thread function for starting and stopping presentation mode */
    static int presentationFunction(void * data);

private:
    /** contains image data */
    CData _data;

    /** trigger for presentation mode*/
    bool _presentation;

    /** trigger for main loop */
    bool _exit;

    /** font and glyph atlas for rendering text */
    TTF_Font *_font;
    CAtlas _atlas;

    /** textures and corresponding rectangles for rendering ASCII images */
    std::vector<SDL_Texture *> _vecTexture;
    std::vector<SDL_Rect> _vecRect;

    /** index of currently used image */
    int _idx;

    /**
     * SDL2 variables for GUI
     * 1        - plain image
     * 2        - ASCII image
     * window   - contains info about position and size
     * renderer - handles rendering for given window
     * surface  - image with all data
     * texture  - SDL2 image in GPU for rendering
     */
    SDL_Window *_window1;
    SDL_Window *_window2;
    SDL_Renderer  * _renderer1;
    SDL_Renderer * _renderer2;
    SDL_Texture * _texture1;
    SDL_Surface * _img1;

    /** helper color */
    SDL_Color _black = {0, 0, 0,0};
    SDL_Color _white = {255, 255, 255, 0};
};




#endif
