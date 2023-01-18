#pragma once

#include <set>
#include "Util/structures.h"

class Animation;
class EntityAnimation;

class Drawer {
    struct Drawable {
        enum class Type {
            ANIMATION,
            ENTITY_ANIMATION
        };

        enum class PositionMode {
            INTEGER,
            DOUBLE
        };

        Type type;
        union {
            const Animation& anim;
            const EntityAnimation& eanim;
        };

        PositionMode position_mode;
        union {
            const Kuribrawl::Vector& pos;
            const Kuribrawl::VectorDouble& posD;
        }

        int depth;

        int operator<=>(const Drawable& other) const{
            return depth - other.depth;
        }
    };
    
    std::set<Drawable> drawables;



};