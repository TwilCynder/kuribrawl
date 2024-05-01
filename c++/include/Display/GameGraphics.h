#pragma once

#include <vector>
#include <functional>
#include <iterator>

#include "Util/util.h"

class DepthfulEntity;
class SDL_Renderer;
class Camera;

class GameGraphics {
    std::vector<std::reference_wrapper<DepthfulEntity>> entities;

    public:
    void clear();

    void add(DepthfulEntity&);
    template <class Range> requires Kuribrawl::RefRangeOf<Range, DepthfulEntity&>
    void add(Range& r){
        for (auto& elt : r){
            add(elt);
        }
    }

    void draw(SDL_Renderer* target, const Camera& cam);
};