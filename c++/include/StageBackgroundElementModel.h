#pragma once
#include "Util/structures.h"

class Animation;

struct StageBackgroundElementModel {
    const Animation& animation;
    Kuribrawl::Vector position;
    int depth;

    StageBackgroundElementModel(const Animation& anim);
};