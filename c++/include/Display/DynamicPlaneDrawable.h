#pragma once

#include "Display/Drawable.h"

class DynamicPlaneDrawable : public Drawable {
    public:
    enum Layer {
        BACKGROUND, //Always drawn in the back
        MIDDLE, //where most dynamic elements will be
        FORGROUND_DECORATION, //where small decorations that appear 
        ABOVE_FOREGROUND_DECORATION, //sometimes dynamic elements need to go above the small decorations
        FOREGROUND //always drawn above all else
    };

    friend class DynamicDrawablePlane;

    DynamicPlaneDrawable(); 
    DynamicPlaneDrawable(const Layer layer, const int level);

    void resetDepth();
    void setLayer(Layer);
    void setLevel(const int);
    void incrLevel();

    private:
    Layer layer;
    int level;
};