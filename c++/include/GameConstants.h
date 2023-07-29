#include "defs.h"
#include "GameConstantsBase.h"

constexpr struct {
    double camera_max_speed_squared = game_constants.camera_max_speed * game_constants.camera_max_speed;
    Kuribrawl::Rectangle camera_nomove_area = { //this is an offset from camera pos
        .left  =(int)(-SCREEN_WIDTH * (game_constants.camera_nomove_area_rel.left / 2)),
        .top   =(int)(SCREEN_HEIGHT * (0.5 + game_constants.camera_nomove_area_rel.top / 2)),
        .right =(int)(SCREEN_WIDTH * (game_constants.camera_nomove_area_rel.right / 2)),
        .bottom=(int)(SCREEN_HEIGHT * (0.5 - game_constants.camera_nomove_area_rel.bottom / 2)),
    };
    Kuribrawl::Vector camera_nomove_area_size = {camera_nomove_area.right - camera_nomove_area.left, camera_nomove_area.top - camera_nomove_area.bottom};
} game_constants_calc;