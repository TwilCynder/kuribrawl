#include "Util/structures.h"

constexpr struct {
    double camera_max_speed = 10;
    Kuribrawl::Rect<double> camera_nomove_area_rel = {
        .left =     0.5, 
        .top =      0.5, 
        .right =    0.5,
        .bottom =   0.5
    };
} game_constants;

constexpr double camera_max_speed_squared = game_constants.camera_max_speed * game_constants.camera_max_speed;