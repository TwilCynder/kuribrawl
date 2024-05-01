#include "DepthfulEntity.h"

DepthfulEntity::DepthUnion::DepthUnion(depth_t depth, subDepth_t subDepth):
    parallax{depth, subDepth}
{}

DepthfulEntity::DepthUnion::DepthUnion(Layer layer, level_t level):
    layer{layer, level}
{}

DepthfulEntity::DepthfulEntity():
    Entity(),
    depth_type(DepthType::LAYER),
    depth{Types::MIDDLE, 0}
{
}

DepthfulEntity::DepthfulEntity(depth_t depth_, subDepth_t subDepth_):
    Entity(),
    depth_type(DepthType::PARALLAX),
    depth{depth_, subDepth_}
{
}

DepthfulEntity::DepthfulEntity(Layer layer_, level_t level_):
    Entity(),
    depth_type(DepthType::PARALLAX),
    depth{layer_, level_}
{
}

inline DepthfulEntity::DepthType DepthfulEntity::getDepthType() const
{
    return depth_type;
}

inline DepthfulEntity::Layer DepthfulEntity::getLayer() const
{
    return depth.layer.layer;
}

inline DepthfulEntity::level_t DepthfulEntity::getLevel() const
{
    return depth.layer.level;
}

inline DepthfulEntity::depth_t DepthfulEntity::getDepth() const
{
    return depth.parallax.depth;
}

inline DepthfulEntity::subDepth_t DepthfulEntity::getSubDepth() const
{
    return depth.parallax.subDepth;
}

bool DepthfulEntityComparator::operator()(const DepthfulEntity & a, const DepthfulEntity & b) const
{
    return 
        (a.depth_type == DepthfulEntity::DepthType::LAYER ? 
            (b.depth_type == DepthfulEntity::DepthType::LAYER ?
                compareLayers(a.depth.layer, b.depth.layer) : //layer vs layer
                !isBelowMainPlane(b.depth.parallax)) : //layer vs parallax
            b.depth_type == DepthfulEntity::DepthType::LAYER ? //parallax vs ?
                isBelowMainPlane(a.depth.parallax) ://parallax vs layer
                compareParallax(a.depth.parallax, b.depth.parallax)
        );
}

bool DepthfulEntityComparator::isBelowMainPlane(const DepthfulEntity::DepthUnion::ParallaxDepth & depth) const
{
    return depth.depth > 1.0 || (depth.depth == 1.0 && depth.subDepth < 1);
}

bool DepthfulEntityComparator::compareParallax(const DepthfulEntity::DepthUnion::ParallaxDepth & a, const DepthfulEntity::DepthUnion::ParallaxDepth & b) const
{
    return a.depth == b.depth ? a.subDepth < b.subDepth : a.depth < b.depth;
}

bool DepthfulEntityComparator::compareLayers(const DepthfulEntity::DepthUnion::LayerDepth & a, const DepthfulEntity::DepthUnion::LayerDepth & b) const
{
    return a.layer == b.layer ? a.level < b.level : a.layer < b.layer;
}
