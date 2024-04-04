#pragma once
#include "Util/structures.h"

class Animation;

struct StageBackgroundElementModel {
    const Animation& animation;
    Kuribrawl::Vector position;
    double depth;

    StageBackgroundElementModel(const Animation& anim);
};