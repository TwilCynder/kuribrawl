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
    cerr << "Uncaught exception : " << exception.what() << '\n';
    exit (1);
}