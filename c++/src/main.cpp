#include <iostream>
#include "SDL2/SDL.h"
#include "app.h"
#include "Debug.h"
#include "DebugInput.h"


int main(int argc, char** argv) try {
    try {
        App app;
        app.init();
        app.loop();
    } catch (KBFatal& exception){
        cerr << "Uncaught exception : \033[0;31m" << exception.what() << "\033[0m\n";
        exit (1);
    }

    return 0;

} catch (KBFatal& exception){
    cerr << "Uncaught exception : \033[0;31m" << exception.what() << "\033[0m\n";
    exit (1);
}