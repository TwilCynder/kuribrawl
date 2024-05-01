#pragma once
#include "Util/structures.h"
#include "Types/GameTypes.h"

class Animation;

struct StageBackgroundElementModel {
    const Animation& animation;
    Kuribrawl::Vector position;
    Types::depth_t depth;
    Types::subDepth_t subdepth;

    StageBackgroundElementModel(const Animation& anim);
};