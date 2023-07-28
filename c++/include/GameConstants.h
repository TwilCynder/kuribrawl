#include "Util/structures.h"

constexpr struct {
    double camera_max_speed = 1;
    Kuribrawl::Rect<double> camera_nomove_area = {0.5, 0.5, 0.5, 0.5};
} gameConstants;

constexpr double camera_max_speed_squared = gameConstants.camera_max_speed * gameConstants.camera_max_speed;