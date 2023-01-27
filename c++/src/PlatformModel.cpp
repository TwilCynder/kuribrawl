#include "PlatformModel.h"

PlatformModel::PlatformModel():
    half_w(0), pos{0, 0}, animation(nullptr)
{}

PlatformModel::PlatformModel(int w_, int x_, int y_) :
    half_w(w_ / 2), pos{x_, y_}, animation(nullptr)
{}

PlatformModel::PlatformModel(int w_, int x_, int y_, const Animation& anim_) :
    half_w(w_ / 2), pos{x_, y_}, animation(&anim_)
{}