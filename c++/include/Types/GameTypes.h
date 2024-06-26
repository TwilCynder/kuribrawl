#pragma once

namespace Types {
    using depth_t = double;
    using subDepth_t = long;
    using level_t = long;

    enum Layer {
        BACKGROUND, //Always drawn in the back
        MIDDLE, //where most dynamic elements will be
        FORGROUND_DECORATION, //where small decorations that appear 
        ABOVE_FOREGROUND_DECORATION, //sometimes dynamic elements need to go above the small decorations
        FOREGROUND //always drawn above all else
    };
}