#include <iostream>
#include "SDL2/SDL.h"
#include "app.h"

using namespace std;

int main(int argc, char **argv)
{
    App app;

    app.init();
    app.loop();

    return 0;
}