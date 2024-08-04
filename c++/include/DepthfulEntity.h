#pragma once

#include "Types/GameTypes.h"
#include "Entity.h"
#include "Display/Drawable.h"
#include <ostream>

using namespace Kuribrawl;

/**
 * @brief An in-game entity that has a depth in the 3D space.
 *
 * Although the game is not rendered using a 3D rendering algorithm, and the camera works in a typically 2D way (2D-only position, only knows the size of the screen, etc), the game uses a parallax effect to create the illusion of a 3D space.
 * The DepthfulEntity class manages all properties related to this virtual 3D space effect / parallax, as well as sorting its instances based on depth.
 * The game's space features a "Main plane", where most entities (and more importantly almost all actual gameplay-related, non-purely-decorative entities) are. DepthfulEntities can either be on that plane (Layer Depth), where their depth is a layer system, used to sort them on the plane, without creating any parallax effet, or outside of it  (Parallax Depth), with an actual non-null virtual depth that creates a parallax effects.
 *
 * The depth value of the Main Plane is 1.0.
 *
 * /!\ It is very important to note that the parallax effect only effects the position of elements on screen : no zooming or dezooming is done. This means that if a sprite is 100 pixels wide, it will be 100 pixels wide on the screen no matter how far it is from the camera, which can cause unexpected results. This is intended, as (de)zooming pixel art sprites often yields ugly results : the user is instead supposed to keep the final depth of an object in mind when designing and drawing it : if you make an object that, in terms of real in-world size, would appear 100px wide on the Main Plane, and you're gonna place it at a depth of 2.0 (twice as far as the Main Plane), you need to make it 50px wide.
 * 
 * Initializing an object with a Parallax-based depth that is less or equal to zero will convert it to infinity, which means the element never moves from it's position on screen no matter where the camera moves. 
 */
class DepthfulEntity : public Entity, public Drawable {
    public:

    using Layer = Types::Layer;

    using depth_t = Types::depth_t;
    using subDepth_t = Types::subDepth_t;
    using level_t = Types::subDepth_t;
    
    static constexpr depth_t infinite_depth = Types::infinite_depth;

    /// @brief Whether an entity is on the Main Plane, or outside of it (with a parallax effect)
    enum class DepthType {
        PARALLAX,
        LAYER
    };

    protected:
    DepthType depth_type; ///< Type of depth for that entity (see DepthType)

    /**
     * @brief Contains the actual depth, which, depending on the depth_type, is either a layer or a distance from the camera.
     */
    union DepthUnion {
        /**
         * @brief Depth for parallax-depth entities
         */
        struct ParallaxDepth {
            /**
             *  @brief The actual depth, or ditance to the camera, that creates a parallax effect.
             *
             * The unit is the distance from the camera to the Main plane, meaning that a depth of 2.0 is twice as far from the camera as the Main plane, and a depth of 0.5 is right between the plane and the camera.
             */
            depth_t depth;
            subDepth_t subDepth; ///< An arbitrary number that has no bearing on the parallax effect, used only to sort same-depth entities (higher subDepth is drawn below)
        } parallax;

        struct LayerDepth {
            Layer layer;    ///< The gameplay layer this entity is on. See Types::Layer for an explanation on gameplay layers.
            level_t level;  ///< An arbitrary number, used to sort same-layer entities (higher level is drawn below)
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

    static depth_t validateDepth(depth_t);
};

/**
 * @brief Comparator for two DepthfulEntities.  
 * 
 * Order is : 
 * - Parallax entities with depth > 1 or equal to 1 with subDepth > 0, sorted by higher depth then higher subdepth
 * - Layer entities, sorted by layer then higher level
 * - Other parallax entities, sorted by higher depth then higher subdepth
 * 
 * o make this intuitive, you can consider that all Layer entities are on a single plane that has a depth of 1.0 and a subdepth of 0.5
 */
class DepthfulEntityComparator {
    public:

    bool operator()(const DepthfulEntity&, const DepthfulEntity&) const;
    static bool isBelowMainPlane(const DepthfulEntity::DepthUnion::ParallaxDepth&);
    static bool compareParallax(const DepthfulEntity::DepthUnion::ParallaxDepth&, const DepthfulEntity::DepthUnion::ParallaxDepth&);
    static bool compareLayers(const DepthfulEntity::DepthUnion::LayerDepth&, const DepthfulEntity::DepthUnion::LayerDepth&);
};