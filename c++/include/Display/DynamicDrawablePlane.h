#pragma once
#include <list>
#include "Display/DynamicPlaneDrawable.h"

class DynamicDrawablePlane : public Drawable {
    public:

    private:
    struct Element {
        DynamicPlaneDrawable& drawable;

        int operator<=>(const Element& other) const{
            return (drawable.layer == other.drawable.layer) ? drawable.level - other.drawable.level : drawable.layer - other.drawable.layer;
        }
    };

    std::list<Element> drawables;

    public:
    void add(DynamicPlaneDrawable&, const DynamicPlaneDrawable::Layer layer, const int level);
    void add(DynamicPlaneDrawable&, const DynamicPlaneDrawable::Layer layer);
    void add(DynamicPlaneDrawable&);

    void sort();

    void draw(SDL_Renderer*, const Camera&) const override;
};