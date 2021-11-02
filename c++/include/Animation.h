#pragma once
#include <memory>
#include "SDL2/SDL.h"
#include "Frame.h"
#include "util.h"

using namespace Kuribrawl;
struct Frame;

/**
 * @brief An animation (i.e.\ an ensemble of frames).
 *
 * Animations are composed of a source image and an array of frames, which are just rectangle positions on the source image.
 */

template <class FrameType>
class Animation {
    friend class AnimationPlayer;

    public:
        enum class EndAction {
        REPEAT,
        START_ANIMATION,
        RETURN_CODE
    };;

    using EndActionData = union {
        const Animation<FrameType>* next_anim; /**< Animation that will be started when this one finishes.
                            Poiter Validity : can be invalidated if the target animation if moved or destroyed (normal behavior should not make this happen)*/
        int code;   ///< A code to be passed to the caller of advance()
    };

    Animation();
    Animation(SDL_Texture* spritesheet);
    Animation(SDL_Texture* spritesheet, int nFrames);
    ~Animation();

    void setSpritesheet(SDL_Texture* spritesheet);
    SDL_Texture* getSpritesheet() const ;
    void initFrames(int n);
    bool is_initialized()const;
    int getNbFrames();
    FrameType* getFrame(int n); //Pointer validity : frames are stored in a unique pointer, can't be invalid as long as returns a frame of this animation
    void setBaseSpeed(double speed);
    double getBaseSpeed();

    void draw(SDL_Renderer* target, int x, int y, int frame)const;
    void draw(SDL_Renderer* target, int x, int y, int frame, int facing)const;

    //Construction
    void setEndAction();
    void setEndAction(const Animation<FrameType>* anim);
    void setEndAction(int code);
    void setEndAction(Animation::EndAction mode, Animation::EndActionData action);
    void setNextAnimation(const Animation<FrameType>*);
    const Animation<FrameType>* getNextAnimation() const;


    protected:
    int nb_frames;  ///< Number of frames in this animation.
    std::unique_ptr<FrameType[]> frames; ///<Array (Basic array unique-pointed) containing the frames (= positions of the frames on the image) of this animation.
    
    private:
    SDL_Texture* spritesheet;       ///<SDL Texture used as the source image.
    SDL_Texture* spritesheet_left;  //KEKW ///< SDL Texture used as the alternative source image for entities that can be oriented in two different directions (typically Fighters)
    Vector display; ///< Size of the source image.

    double base_speed;  /**< Speed of this animation.
                        Can be < 1, in which case it will be used as a multiplier; or an integer, in which case it will be the total duration of the Animation.*/

    EndAction end_action_mode;
    EndActionData end_action;
};

using BasicAnimation = Animation<Frame>;