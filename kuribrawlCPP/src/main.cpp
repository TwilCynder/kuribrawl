#include "SDL2/SDL.h"
#include "app.h"

int main(int argc, char **argv)
{
    App app = App();

    app.init();
    app.loop();

    return 0;
}