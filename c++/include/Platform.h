#pragma once

#include "Util/util.h"
#include "Animation.h"

struct PlatformModel {
    int w;
    Kuribrawl::Vector pos;

    PlatformModel();
    PlatformModel(int w, int x = 0, int y = 0);
    PlatformModel(int w, int x, int y, const Animation*);

    const Animation* animation;
};