#pragma once

#include "Types/GameTypes.h"
#include "Entity.h"
#include "Display/Drawable.h"
#include <ostream>


class DepthfulEntity : public Entity, public Drawable {
    public:

    using Layer = Types::Layer;

    using depth_t = Types::depth_t;
    using subDepth_t = Types::subDepth_t;
    using level_t = Types::subDepth_t;

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

    std::ostream& displayInfo(std::ostream&) const;

    protected:
    void displayLayerDepthInfo(std::ostream&) const;
    void displayParallaxDepthInfo(std::ostream&) const;

    int getParallaxXOnScreen(const Camera&, int x) const;
    int getParallaxYOnScreen(const Camera&, int y) const;
    int getMainplaneXOnScreen(const Camera&, int x) const;
    int getMainplaneYOnScreen(const Camera&, int y) const;
    int getXOnScreen(const Camera&, int x) const;
    int getYOnScreen(const Camera&, int y) const;
};

class DepthfulEntityComparator {
    public:

    bool operator()(const DepthfulEntity&, const DepthfulEntity&) const;
    bool isBelowMainPlane(const DepthfulEntity::DepthUnion::ParallaxDepth&) const;
    bool compareParallax(const DepthfulEntity::DepthUnion::ParallaxDepth&, const DepthfulEntity::DepthUnion::ParallaxDepth&) const;
    bool compareLayers(const DepthfulEntity::DepthUnion::LayerDepth&, const DepthfulEntity::DepthUnion::LayerDepth&) const;
};