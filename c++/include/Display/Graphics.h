#pragma once

#include "Display/Drawer.h"
#include "Display/Drawable.h"
#include "Display/DynamicDrawablePlane.h"
#include "Display/DynamicPlaneDrawable.h"

class Graphics {
    Drawer drawer;
    DynamicDrawablePlane main_plane;

    public:
    Graphics();

    void add(DynamicPlaneDrawable& d, const DynamicPlaneDrawable::Layer layer = DynamicPlaneDrawable::Layer::MIDDLE, const int level = 0);
    void addToMainPlane(DynamicPlaneDrawable& d, const DynamicPlaneDrawable::Layer layer = DynamicPlaneDrawable::Layer::MIDDLE, const int level = 0);
    void add(const Drawable& d, const double depth = 1.0, const int subdepth = 0);

    void draw(SDL_Renderer*, const Camera&) ;
};