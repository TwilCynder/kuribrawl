#include "Display/DynamicDrawablePlane.h"

void DynamicDrawablePlane::add(DynamicPlaneDrawable& d, const DynamicPlaneDrawable::Layer layer, const int level)
{
    d.setLevel(level);
    add(d, layer);
}

void DynamicDrawablePlane::add(DynamicPlaneDrawable &d, const DynamicPlaneDrawable::Layer layer)
{
    d.setLayer(layer);
    add(d);
}

void DynamicDrawablePlane::add(DynamicPlaneDrawable &d)
{
    drawables.emplace_back(d);
}

void DynamicDrawablePlane::sort(){
    drawables.sort();
}

void DynamicDrawablePlane::draw(SDL_Renderer* target, const Camera& camera) const{
    for (const Element& e : drawables){
        e.drawable.incrLevel();
        e.drawable.draw(target, camera);
    }
}