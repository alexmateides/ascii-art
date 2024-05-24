#include "app.h"

CApp::CApp(CData & data) : _data(data), _exit(false), _presentation(false){
    /** SDL setup */
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    TTF_Init();

    /** GUI setup */
    _idx = 0;
    _img1 = _data._vecSurface[_idx];
    _window1 = SDL_CreateWindow("Original Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _img1->w, _img1->h,SDL_WINDOW_RESIZABLE|SDL_WINDOW_HIDDEN);
    _renderer1 = SDL_CreateRenderer(_window1, -1, 0);
    _window2 = SDL_CreateWindow("ASCII preview", 0, 0, _img1->w, _img1->h,SDL_WINDOW_RESIZABLE|SDL_WINDOW_HIDDEN);
    _renderer2 = SDL_CreateRenderer(_window2, -1, 0);
    _texture1 =  SDL_CreateTexture(_renderer1, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, _img1->w,_img1->h);

    /** glyph atlas setup */
    _font = TTF_OpenFont(FONT_PATH, FONTSIZE_DISPLAY);
    auto vec = _data.getAlphabet();
    _atlas = CAtlas(vec, _font, _renderer2);
}

CApp::~CApp() {
    /** cleanup of allocated resources */
    for (auto texture : _vecTexture) {
        SDL_DestroyTexture(texture);
    }
    _data.freeSurfaces();
    _atlas.freeGlyphs();

    /** SDL process termination */
    SDL_DestroyWindow(_window1);
    SDL_DestroyWindow(_window2);
    SDL_DestroyRenderer(_renderer1);
    SDL_DestroyRenderer(_renderer2);
    TTF_CloseFont(_font);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}
void CApp::mainLoop() {
    SDL_Event event;
    std::ofstream fileStream;
    std::vector<uint32_t> vecIdx = _data.getIdx(_data._vecSurface[0]);
    processAll();
    SDL_ShowWindow(_window1);

    /** default filter setup **/
    auto negativeFilter = std::make_shared<CNegative>();
    auto verticalFilter = std::make_shared<CReflectionVertical>();
    auto horizontalFilter = std::make_shared<CReflectionHorizontal>();
    auto brightnessUpscale = _data.defaultBrightness();
    auto brightnessDownscale = brightnessUpscale->inverseCopy();
    auto rotationFilter = _data.defaultRotate();
    auto antiRotationFilter = rotationFilter->inverseCopy();
    auto resizeFilter = _data.defaultResize();
    auto antiResizeFilter = resizeFilter->inverseCopy();
    auto convolutionFilter = std::make_shared<CKernel>(_data._defaultBlur);

    while (!_exit) {
        /** presentation mode */
        while(_presentation){
            _idx++;
            switchImage();
            renderAll();
            SDL_Delay(PRESENTATION_DELAY);
        }

        /** render each frame */
        renderAll();

        /** event handling loop */
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_WINDOWEVENT:
                /** handles window exit */
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        _exit = true;
                        break;

                    default:
                        break;
                }
            case SDL_KEYDOWN:
                /** prevents event clumping */
                SDL_PumpEvents();
                SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

                /** key controls */
                switch (event.key.keysym.sym) {
                    /** console output */
                    case SDLK_k:
                        _data.printImg(std::cout, _img1);
                        std::cout << "\n\n\n";
                        break;

                    /** all images into console */
                    case SDLK_l:
                        for (auto img: _data._vecSurface){
                            _data.printImg(std::cout, img);
                            std::cout << "\n\n\n";
                        }
                        break;

                    /** file output */
                    case SDLK_o:
                        fileStream.open(_data._outputPath, std::ios_base::app);
                        _data.printImg(fileStream, _img1);
                        fileStream << "\n\n\n";
                        fileStream.close();
                        break;

                    /** all images into file */
                    case SDLK_p:
                        fileStream.open(_data._outputPath, std::ios_base::app);
                        for (auto img: _data._vecSurface){
                            _data.printImg(fileStream, img);
                            fileStream << "\n\n\n";
                        }
                        fileStream.close();
                        break;

                    /** next img */
                    case SDLK_RIGHT:
                        _idx++;
                        switchImage();
                        break;

                    /** previous img */
                    case SDLK_LEFT:
                        _idx--;
                        switchImage();
                        break;

                    /** default brightness */
                    case SDLK_UP:
                        brightnessUpscale->filter(&_data._vecSurface[_idx]);
                        processImg();
                        switchImage();
                        break;

                    /** reverse default brightness */
                    case SDLK_DOWN:
                        brightnessDownscale->filter(&_data._vecSurface[_idx]);
                        processImg();
                        switchImage();
                        break;

                    /** negative */
                    case SDLK_n:
                        negativeFilter->filter(&_data._vecSurface[_idx]);
                        processImg();
                        switchImage();
                        break;

                    /** flip along Y axis */
                    case SDLK_r:
                        horizontalFilter->filter(&_data._vecSurface[_idx]);
                        processImg();
                        switchImage();
                        break;

                    /** flip along X axis */
                    case SDLK_t:
                        verticalFilter->filter(&_data._vecSurface[_idx]);
                        processImg();
                        switchImage();
                        break;

                    /** default resize */
                    case SDLK_d:
                        resizeFilter->filter(&_data._vecSurface[_idx]);
                        processImg();
                        switchImage();
                        break;

                    /** reverse default resize */
                    case SDLK_f:
                        antiResizeFilter->filter(&_data._vecSurface[_idx]);
                        processImg();
                        switchImage();
                        break;

                    /** default rotate */
                    case SDLK_g:
                        rotationFilter->filter(&_data._vecSurface[_idx]);
                        processImg();
                        switchImage();
                        break;

                    /** reverse default rotate */
                    case SDLK_h:
                       antiRotationFilter->filter(&_data._vecSurface[_idx]);
                        processImg();
                        switchImage();
                        break;

                    case SDLK_c:
                        convolutionFilter->filter(&_data._vecSurface[_idx]);
                        processImg();
                        switchImage();
                        break;


                    /** start/stop presentation mode */
                    case SDLK_SPACE:
                        if (_presentation){
                            _presentation = false;
                        }
                        else{
                            _presentation = true;
                            SDL_CreateThread(presentationFunction, "Presentation", (void *)this);
                        }
                        break;

                    /** quit app */
                    case SDLK_ESCAPE:
                        _exit = true;
                        break;
                }
                break;
            case SDL_QUIT:
                _exit = true;
                break;
        }
    }
}


void CApp::processImg() {
    std::vector<uint32_t> vecIdx = _data.getIdx(_data._vecSurface[_idx]);
    SDL_Texture *texture = _atlas.renderString(_window2,_renderer2, vecIdx, _data._vecSurface[_idx]->w, _data._vecSurface[_idx]->h);
    int textWidth, textHeight;
    SDL_GetWindowSize(_window2, &textWidth, &textHeight);
    textHeight*=2;
    int x, y;
    SDL_GetWindowPosition(_window2, &x, &y);
    SDL_Rect rect;
    rect.x = x; rect.y = y; rect.w = textWidth, rect.h = textHeight;
    if (_vecTexture.size() != _vecRect.size()) {
        throw std::invalid_argument("_vecRect doesnt match _vecTexture");
    }
    if (_vecRect.size() <= _idx) {
        _vecRect.push_back(rect);
        _vecTexture.push_back(texture);
    } else {
        _vecRect[_idx] = rect;
        SDL_DestroyTexture(_vecTexture[_idx]);
        _vecTexture[_idx] = texture;
    }
}


void CApp::processAll() {
    SDL_HideWindow(_window2);
    for (int i = 0; i < _data._vecSurface.size(); i++) {
        processImg();
        _idx++;
    }
    _idx=0;
    SDL_SetWindowPosition(_window2, 0, 0);
    SDL_SetWindowSize(_window2, _vecRect[0].w, _vecRect[0].h/2);
    SDL_ShowWindow(_window2);
}

int CApp::presentationFunction(void * data){
    CApp * app = (CApp*)data;
    SDL_Event event;
    while (app->_presentation){
        SDL_WaitEvent(&event);
        if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE){
            app->_presentation = false;
        }
    }
    return 0;
}

void CApp::switchImage(){
    if (_idx < 0) {
        _idx = _data._vecSurface.size() - 1;
    }
    if (_idx >= _data._vecSurface.size()) {
        _idx = 0;
    }
    _img1 = _data._vecSurface[_idx];
    SDL_DestroyTexture(_texture1);
    SDL_SetWindowSize(_window1, _img1->w, _img1->h);
    _texture1 = SDL_CreateTexture(_renderer1, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, _img1->w, _img1->h);
    SDL_SetWindowPosition(_window2, 0, 0);
    SDL_SetWindowSize(_window2, _vecRect[_idx].w, _vecRect[_idx].h/2);
    SDL_UpdateTexture(_texture1, NULL, _img1->pixels, _img1->w * sizeof(Uint32));
    SDL_RenderCopy(_renderer2, _vecTexture[_idx], NULL, &_vecRect[_idx]);
    SDL_RenderPresent(_renderer2);
    SDL_RenderCopy(_renderer1, _texture1, NULL, NULL);
    SDL_RenderPresent(_renderer1);
}

void CApp::renderAll(){
    SDL_RenderClear(_renderer1);
    SDL_RenderClear(_renderer2);
    SDL_UpdateTexture(_texture1, NULL, _img1->pixels, _img1->w * sizeof(Uint32));
    SDL_RenderCopy(_renderer2, _vecTexture[_idx], NULL, &_vecRect[_idx]);
    SDL_RenderPresent(_renderer2);
    SDL_RenderCopy(_renderer1, _texture1, NULL, NULL);
    SDL_RenderPresent(_renderer1);
}