#include "Platform.h"

Platform::Platform():
    w(0), pos{0, 0}, animation(nullptr)
{}

Platform::Platform(int w_, int x_, int y_) :
    w(w_), pos{x_, y_}, animation(nullptr)
{}

Platform::Platform(int w_, int x_, int y_, const Animation* anim_) :
    w(w_), pos{x_, y_}, animation(anim_)
{}