#pragma once

#include "Entity.h"

class DepthfulEntity : public Entity {
    public:
    enum Layer {
        BACKGROUND, //Always drawn in the back
        MIDDLE, //where most dynamic elements will be
        FORGROUND_DECORATION, //where small decorations that appear 
        ABOVE_FOREGROUND_DECORATION, //sometimes dynamic elements need to go above the small decorations
        FOREGROUND //always drawn above all else
    };

    using depth_t = double;
    using subDepth_t = long;
    using level_t = long;

    enum class DepthType {
        PARALLAX,
        LAYER
    };

    protected:
    DepthType depth_type;

    union DepthUnion {
        struct ParallaxDepth {
            depth_t depth;
            subDepth_t subDepth;
        } parallax;

        struct LayerDepth {
            Layer layer;
            level_t level;
        } layer;

        DepthUnion(depth_t depth, subDepth_t subDepth = 0);
        DepthUnion(Layer layer, level_t level = 0);
    } depth;

    friend class DepthfulEntityComparator;

    public:
    DepthfulEntity();
    DepthfulEntity(depth_t depth, subDepth_t subDepth = 0);
    DepthfulEntity(Layer layer, level_t level = 0);

    inline DepthType getDepthType() const;
    inline Layer getLayer() const;
    inline level_t getLevel() const;
    inline depth_t getDepth() const;
    inline subDepth_t getSubDepth() const;
};

class DepthfulEntityComparator {
    public:

    bool operator()(const DepthfulEntity&, const DepthfulEntity&) const;
    bool isBelowMainPlane(const DepthfulEntity::DepthUnion::ParallaxDepth&) const;
    bool compareParallax(const DepthfulEntity::DepthUnion::ParallaxDepth&, const DepthfulEntity::DepthUnion::ParallaxDepth&) const;
    bool compareLayers(const DepthfulEntity::DepthUnion::LayerDepth&, const DepthfulEntity::DepthUnion::LayerDepth&) const;
};