#pragma once

#include <set>
#include "Util/structures.h"
#include "Drawable.h"

class SDL_Renderer;

class Drawer {

    struct Element {
        const Drawable& drawable;
        const int depth;

        int operator<=>(const Element& other) const{
            return depth - other.depth;
        }
    };

    std::multiset<Element> drawables;

    public:
    void add(const Drawable&, const int depth);
    void add(const Drawable&);

    void draw(SDL_Renderer*, const Camera&);

        /*
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
        };

        int depth;

        int operator<=>(const Drawable& other) const{
            return depth - other.depth;
        }
    };
    */
};


    