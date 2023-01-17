#pragma once

#include <set>

class Animation;
class EntityAnimation;

class Drawer {
    struct Drawable {
        enum class Type {
            ANIMATION,
            ENTITY_ANIMATION
        };

        Type type;

        union {
            const Animation& anim;
            const EntityAnimation& eanim;
        };
        int depth;

        int operator<=>(const Drawable& other) const{
            return depth - other.depth;
        }
    };
    
    std::set<Drawable> drawables;

    

};