#include <iostream>
#include "SDL2/SDL.h"
#include "app.h"
#include "Debug.h"

using namespace std;

int main(int argc, char **argv)
{
    App app;

    try {
        
        app.init();
        app.loop();

    } catch (KBFatal& e){
        cerr << e.what();
    }

    return 0;
}