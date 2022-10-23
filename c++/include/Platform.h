#include "Util/util.h"
#include "Animation.h"

struct Platform {
    int w;
    Kuribrawl::Vector pos;

    Platform();
    Platform(int w, int x, int y);
    Platform(int w, int x, int y, const Animation*);

    private:
    const Animation* animation;
};