#pragma once
#include "Util/structures.h"

class Animation;

struct StageBackgroundElement {
    const Animation& animation;
    Kuribrawl::Vector position;
    int depth;

    StageBackgroundElement(const Animation& anim);
};