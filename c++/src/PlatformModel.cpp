#include "PlatformModel.h"

PlatformModel::PlatformModel():
    w(0), pos{0, 0}, animation(nullptr)
{}

PlatformModel::PlatformModel(int w_, int x_, int y_) :
    w(w_), pos{x_, y_}, animation(nullptr)
{}

PlatformModel::PlatformModel(int w_, int x_, int y_, const Animation& anim_) :
    w(w_), pos{x_, y_}, animation(&anim_)
{}