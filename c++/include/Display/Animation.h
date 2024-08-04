#pragma once
#include <memory>
#include <compare>
#include "SDL2/SDL.h"
#include "Frame.h"
#include "Util/util.h"
#include "AnimationEndAction.h"
#include "Types/Data.h"

using namespace Kuribrawl;
using namespace Kuribrawl::Types;
struct Frame;

/**
 * @brief An animation (i.e.\ an ensemble of frames).
 *
 * Animations are composed of a source image and an array of frames, which are just rectangle positions on the source image.
 */

class AnimationBase {
    public:
    friend class AnimationPlayerBase;

    void initFrames(frame_index_t n);

    AnimationBase();
    AnimationBase(SDL_Texture* spritesheet);
    AnimationBase(SDL_Texture* spritesheet, int nFrames);
    ~AnimationBase();

    void setSpritesheet(SDL_Texture* spritesheet);
    SDL_Texture* getSpritesheet() const ;
    bool is_initialized()const;
    frame_index_t getNbFrames();
    Frame* getFrame(frame_index_t n); //Pointer validity : frames are stored in a unique pointer, can't be invalid as long as returns a frame of this animation
    void setBaseSpeed(double speed);
    double getBaseSpeed();

    void draw(SDL_Renderer* target, int x, int y, frame_index_t frame)const;
    void draw(SDL_Renderer* target, int x, int y, frame_index_t frame, Kuribrawl::Side facing)const;

    bool operator==(const AnimationBase&) const;
    std::weak_ordering operator<=>(const AnimationBase&) const;

    protected:
    int nb_frames;  ///< Number of frames in this animation.
    bool loop; ///deprecated in favor of endactions

    private:
    SDL_Texture* spritesheet;       ///<SDL Texture used as the source image.
    SDL_Texture* spritesheet_left;  ///< SDL Texture used as the alternative source image for entities that can be oriented in two different directions (typically Fighters)
    std::unique_ptr<Frame[]> frames; ///<Array (Basic array unique-pointed) containing the frames (= positions of the frames on the image) of this animation.
    Vector display; ///< Size of the source image.

    double base_speed;  /**< Speed of this animation.
                        Can be < 1, in which case it will be used as a multiplier; or a >0 integer, in which case it will be the total duration of the Animation.*/
    
};

class Animation : public AnimationBase, public AnimationEndActionOwner<Animation> {
    public:
    template <typename... Args>
    Animation(Args...args):
        AnimationBase(args...), AnimationEndActionOwner(EndAction::repeat_tag)
    {}

    friend class AnimationPlayer;
};