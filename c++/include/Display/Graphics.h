#pragma once

#include <vector>
#include <algorithm>

class Drawable;

class Graphics {
    std::vector<Drawable&> drawables;

    public:
    void clear();

    void add(const Drawable&);

    void draw();
};