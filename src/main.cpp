#include <iostream>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <fstream>
#include "data.h"
#include "filter.h"
#include "load.h"
#include "../libs/json.hpp"
#include "app.h"
#include "tests.h"

/** driver code */
int main(int argc, char* argv[]) {
    /** check for valid arguments */
    if (argc != 2){
        std::cout << "INVALID NUMBER OF ARGUMENTS\n";
        return EXIT_FAILURE;
    }

    /** unit tests */
    loadTest();


    CData data;
    CLoader loader(argv[1]);
    assert(loader.parse(data));
    CApp app(data);
    app.mainLoop();
    return EXIT_SUCCESS;
}


