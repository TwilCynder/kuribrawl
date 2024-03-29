#include "Util/structures.h"

constexpr struct {
    double camera_max_speed = 5;
    Kuribrawl::Rect<double> camera_nomove_area_rel = {
        .left =     0.4, 
        .top =      0.25, 
        .right =    0.4,
        .bottom =   0.25
    };

    double camera_offset_y = 0.15;

    int input_durability = 10;
} game_constants;

constexpr double camera_max_speed_squared = game_constants.camera_max_speed * game_constants.camera_max_speed;