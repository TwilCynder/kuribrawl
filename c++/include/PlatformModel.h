#pragma once

#include "Util/util.h"
#include "Display/Animation.h"

struct PlatformModel {
    int half_w;
    Kuribrawl::Vector pos;

    PlatformModel();
    PlatformModel(int w, int x = 0, int y = 0);
    PlatformModel(int w, int x, int y, const Animation&);

    const Animation* animation;
    bool traversable_downwards;
};