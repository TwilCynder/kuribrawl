#pragma once

#include "Util/structures.h"

class Camera {
    public:
    /// @brief Position of the camera. It is the point of stage that is going to be displayed in the bottom middle point of the screen.
    Kuribrawl::Vector position;
    Camera();
    int getXOnScreen(int x) const;
    int getYOnScreen(int y) const;

    int getXOnScreen(int x, double depth) const;
    int getYOnScreen(int y, double depth) const;
};