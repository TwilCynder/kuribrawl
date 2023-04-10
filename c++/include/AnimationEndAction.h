#pragma once

template <class A>
struct AnimationEndAction {
    // okay look this is a struct nothing is private but you SHOULD NOT modify the fields yourself
    //just read them and use the "set()" methods
    //your fault if you fuck it up
    static struct repeat{} repeat_tag;

    enum class Mode {
        REPEAT,
        NONE,
        START_ANIMATION,
        RETURN_CODE
    } mode;

    union Data {
        const A* next_anim; /**< Animation that should be started by the AnimationPlayer when this one finishes.
                            Poiter Validity : can be invalidated if the target animation if moved or destroyed (normal behavior should not make this happen)*/
        int code;   ///< A code to be passed to the caller of advance()
        
        Data() : next_anim(nullptr) {}
        Data(const A* anim) : next_anim(anim) {}
        Data(int code_) : code(code_) {}
    } data;

    AnimationEndAction(const A* anim) : mode(Mode::START_ANIMATION), data(anim) {}
    AnimationEndAction(int code_) : mode(Mode::RETURN_CODE), data(code_)   {}
    AnimationEndAction(AnimationEndAction::repeat) : mode(Mode::REPEAT), data()   {}
    AnimationEndAction() : mode(Mode::NONE), data()   {}

    void set(){
        mode = Mode::NONE;
    }
    void set(repeat){
        mode = Mode::REPEAT;
    }
    void set(int code_){
        mode = Mode::RETURN_CODE;
        data.code = code_;
    }
    void set(const A* anim_){
        mode = Mode::NONE;
        data.next_anim = anim_ ;
    }

    template<typename T>
    AnimationEndAction& operator =(T arg){
        set(arg);
        return *this;
    }
};

template <class A, typename... Args>
concept EntityAnimationEndActionValidArg = requires (Args ...args, AnimationEndAction<A> ea) {
    ea.set(args...);
};

template <class A>
class AnimationEndActionOwner {
    protected:
    using EndAction = AnimationEndAction<A>;
    EndAction end_action;

     public:
    template <typename... Args>
    void setEndAction(Args...args){
        end_action.set(args...);
    }

    template <typename... Args>
    AnimationEndActionOwner(Args...args):
        end_action(args...)
    {}
};