#include "DepthfulEntity.h"
#include "Camera.h"
#include <cmath>

/**
 * @brief Construct a new instance of the DepthUnion union, populating the Parallax Depth member.
 */
DepthfulEntity::DepthUnion::DepthUnion(depth_t depth, subDepth_t subDepth):
    parallax{validateDepth(depth), subDepth}
{}

/**
 * @brief Construct a new instance of the DepthUnion union, populating the Layer Depth member.
 */
DepthfulEntity::DepthUnion::DepthUnion(Layer layer, level_t level):
    layer{layer, level}
{}

/**
 * @brief Construct a new, default DepthfulEntity, with Layer-based depth. 
 */
DepthfulEntity::DepthfulEntity():
    Entity(),
    depth_type(DepthType::LAYER),
    depth{Types::MIDDLE, 0}
{
}

/**
 * @brief Construct a new DepthfulEntity, configuring a Parallax-based depth.  
 */
DepthfulEntity::DepthfulEntity(depth_t depth_, subDepth_t subDepth_):
    Entity(),
    depth_type(DepthType::PARALLAX),
    depth{validateDepth(depth_), subDepth_}
{
}

/**
 * @brief Construct a new DepthfulEntity, configuring a Layer-based depth.  
 */
DepthfulEntity::DepthfulEntity(Layer layer_, level_t level_):
    Entity(),
    depth_type(DepthType::LAYER),
    depth{layer_, level_}
{
}

inline DepthfulEntity::DepthType DepthfulEntity::getDepthType() const
{
    return depth_type;
}

/**
 * @brief Returns the layer this entity is on, assuming is uses Layer-based depth
 * @return DepthfulEntity::Layer 
 */
inline DepthfulEntity::Layer DepthfulEntity::getLayer() const
{
    return depth.layer.layer;
}

/**
 * @brief Returns the level this entity is on, assuming is uses Layer-based depth
 * @return DepthfulEntity::level_t 
 */
inline DepthfulEntity::level_t DepthfulEntity::getLevel() const
{
    return depth.layer.level;
}

/**
 * @brief Returns the parallax depth, assuming is uses parallax-based depth
 * @return DepthfulEntity::depth_t 
 */
inline DepthfulEntity::depth_t DepthfulEntity::getDepth() const
{
    return depth.parallax.depth;
}

/**
 * @brief Returns the parallax subdepth, assuming is uses parallax-based depth
 * @return DepthfulEntity::subDepth_t 
 */
inline DepthfulEntity::subDepth_t DepthfulEntity::getSubDepth() const
{
    return depth.parallax.subDepth;
}

/**
 * @brief Writes information about this DepthfulEntity, namely it's depth type, and either layer/level or depth/subdepth, to an ostream.
 * @param os a writable stream
 * @return std::ostream& the input ostream
 */
std::ostream& DepthfulEntity::displayInfo(std::ostream & os) const
{
    os << "DepthfulEntity - ";
    if (depth_type == DepthType::LAYER) {
        displayLayerDepthInfo(os);
    } else {
        displayParallaxDepthInfo(os);
    }

    return os;
}

/**
 * @brief Writes the Layer-based depth properties to an ostream, assuming it's the type of depth this entity uses.
 * @param os a writable stream
 */
void DepthfulEntity::displayLayerDepthInfo(std::ostream & os) const
{
    os
        << "depth type : Layers | layer : "
        << depth.layer.layer
        << " | level : "
        << depth.layer.level
    ;
}

/**
 * @brief Writes the Layer-based depth properties to an ostream, assuming it's the type of depth this entity uses.
 * @param os a writable stream
 */
void DepthfulEntity::displayParallaxDepthInfo(std::ostream & os) const
{
    os
        << "depth type : Parallax | depth : "
        << depth.parallax.depth
        << " | subdepth : "
        << depth.parallax.subDepth
    ;
}

/**
 * @brief Compares two DethfulEntities, according to the order specified in the documentation of this class
 * @return true if a is below b according to this order
 */
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

/**
 * @brief Returns whether a certain depth is below the "main plane", which is where all the Layer entities are
 */
bool DepthfulEntityComparator::isBelowMainPlane(const DepthfulEntity::DepthUnion::ParallaxDepth & depth)
{
    return depth.depth > 1.0 || (depth.depth == 1.0 && depth.subDepth < 1);
}

/**
 * @brief Compares two Parallax-based depths
 * @return true if a is below b according to this order
 */
bool DepthfulEntityComparator::compareParallax(const DepthfulEntity::DepthUnion::ParallaxDepth & a, const DepthfulEntity::DepthUnion::ParallaxDepth & b)
{
    return a.depth == b.depth ? a.subDepth < b.subDepth : (a.depth < b.depth);
}

/**
 * @brief Compares two Layer-based depths
 * @return true if a is below b according to this order
 */
bool DepthfulEntityComparator::compareLayers(const DepthfulEntity::DepthUnion::LayerDepth & a, const DepthfulEntity::DepthUnion::LayerDepth & b)
{
    return a.layer == b.layer ? a.level < b.level : a.layer > b.layer;
}

/**
 * @brief Computes the X position of this entity on the screen, assuming it is using Parallax depth (meaning the position on scren is affected by that depth), based on a certain camera position
 * @param camera the camera used to compute objects positions relative to the screen
 * @param x the in-game X position of this entity
 * @return The X position of this entity on the screen
 */
inline int DepthfulEntity::getParallaxXOnScreen(const Camera & camera, int x) const
{
	return camera.getXOnScreen(x, depth.parallax.depth);
}

/**
 * @brief Computes the Y position of this entity on the screen, assuming it is using Parallax depth (meaning the position on scren is affected by that depth), based on a certain camera position
 * @param camera the camera used to compute objects positions relative to the screen
 * @param x the in-game Y position of this entity
 * @return The Y position of this entity on the screen
 */
int DepthfulEntity::getParallaxYOnScreen(const Camera & camera, int y) const
{
	return camera.getYOnScreen(y, depth.parallax.depth);
}

/**
 * @brief Computes the X position of this entity on the screen, assuming it is using Layer depth (meaning the position on scren is not affected by any parallax effect), based on a certain camera position
 * @param camera the camera used to compute objects positions relative to the screen
 * @param x the in-game X position of this entity
 * @return The X position of this entity on the screen
 */
int DepthfulEntity::getMainplaneXOnScreen(const Camera & camera, int x) const
{
	return camera.getXOnScreen(x);
}

/**
 * @brief Computes the Y position of this entity on the screen, assuming it is using Layer depth (meaning the position on scren is not affected by any parallax effect), based on a certain camera position
 * @param camera the camera used to compute objects positions relative to the screen
 * @param x the in-game Y position of this entity
 * @return The Y position of this entity on the screen
 */
int DepthfulEntity::getMainplaneYOnScreen(const Camera & camera, int y) const
{
	return camera.getXOnScreen(y);
}


int DepthfulEntity::getXOnScreen(const Camera & camera, int x) const
{
	return depth_type == DepthType::PARALLAX ? getParallaxXOnScreen(camera, x) : getMainplaneXOnScreen(camera, x);
}

int DepthfulEntity::getYOnScreen(const Camera & camera, int y) const
{
	return depth_type == DepthType::PARALLAX ? getParallaxYOnScreen(camera, y) : getMainplaneYOnScreen(camera, y);
}

/**
 * @brief Converts a given parallax depth into a valid one. Valid depths are always positive ; if a negative or null value is supplied, it is converted to Infinity
 * @param depth 
 * @return DepthfulEntity::depth_t 
 */
DepthfulEntity::depth_t DepthfulEntity::validateDepth(DepthfulEntity::depth_t depth)
{
    return (depth <= 0) ? infinite_depth : depth;
}
