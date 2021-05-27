#include <iostream>
#include "SDL2/SDL.h"
#include "app.h"
#include "Debug.h"
#include "Random.h"
#include "messageBox.h"
#include <ctime>

int main(int argc, char** argv) try {

    #ifdef _WIN32
        Debug::log("_WIN32");
    #endif
    #ifdef __WIN32__
        Debug::log("__WIN32__");
    #endif
    #ifdef __MINGW32__
        Debug::log("__MINGW32__");
    #endif

    int framerate = 0;

    if (argc > 1){
        framerate = atoi(argv[1]);
    }
    if (framerate == 0) framerate = 60;

    Random::setSeed(std::time(nullptr));

    App app(framerate);
    app.init();
    app.loop();

    return 0;

} catch (KBFatal& exception){
    cerr << "Uncaught exception : \033[0;31m" << exception.what() << "\033[0m\n";

    Kuribrawl::errorPopup(exception.getData(), "Kuribrawl - Erreur", exception.userMessage());
    exit(1);
}

/**
 * \todo Extend the way state animations work (saving a pointer to the animation to avoid map acesses) to other fixed animation like jump
 */