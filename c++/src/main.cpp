#include <iostream>
#include "SDL2/SDL.h"
#include "app.h"
#include "Debug.h"
#include "Random.h"
#include "messageBox.h"
#include <ctime>

Parameters global_parameters = {1};

int main(int argc, char** argv) try {

    int framerate = 0;

    if (argc > 1){
        framerate = atoi(argv[1]);
    }
    if (framerate == 0) framerate = 60;

    if (argc > 2){
        global_parameters.slowness = atoi(argv[2]);
    }

    if (global_parameters.slowness < 1){
        throw KBFatal("Slowness must be at least 1");
    }

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