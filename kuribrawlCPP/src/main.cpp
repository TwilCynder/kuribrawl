#include "SDL2/SDL.h"
#include "app.h"

int main(int argc, char **argv)
{
    App app;

    app.init();

    while(1){
        app.handleEvents();
        SDL_Delay(15);
    }

    return 0;
}