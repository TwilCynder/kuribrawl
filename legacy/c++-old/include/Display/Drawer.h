#pragma once

#include <set>
#include "Util/structures.h"
#include "Display/Drawable.h"

class SDL_Renderer;

class Drawer {

    struct Element {
        const Drawable& drawable;
        const double depth;
        const int subDepth;

        int operator<=>(const Element& other) const {
            return /*(depth == other.depth) ? (layer == other.layer) ? level - other.level : layer - other.layer :*/ depth - other.depth;
        }
    };

    std::multiset<Element> drawables;

    public:
    //void add(const Drawable&, const int depth, const Layer layer, const int level);
    //void add(const Drawable&, const int depth, const Layer layer);
    void add(const Drawable&, const double depth = 1.0, const int subdepth = 0);
    //void add(const Drawable&, const double depth);
    //void add(const Drawable&);

    void draw(SDL_Renderer*, const Camera&) const;

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


    