#include <iostream>
#include "SDL2/SDL.h"
#include "app.h"
#include "Debug.h"
#include "DebugInput.h"

int main(int argc, char** argv) try {
    App app;
    app.init();
    app.loop();

    return 0;

} catch (KBFatal& exception){
    cerr << "Uncaught exception : \033[0;31m" << exception.what() << "\033[0m\n";
    exit (1);
}

/**
 * \todo Extend the way state animations work (saving a pointer to the animation to avoid map acesses) to other fixed animation like jump
 * 
 */