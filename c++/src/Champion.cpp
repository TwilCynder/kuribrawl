#include "Champion.h"
#include "Debug.h"
#include "EntityAnimation.h"

#define DEFAULT_GRAVITY 0.6

/**
 * @brief Construct a new Champion:: Champion object
 *
 * @param name_ The internal name.
 */
Champion::Champion(const std::string& name_):
    name(name_),
    default_animations(std::make_unique<const EntityAnimation*[]>((int)DefaultAnimation::TOTAL)),
    default_moves(std::make_unique<const Move* []>((int)DefaultMoves::TOTAL))
{
    this->val.gravity = DEFAULT_GRAVITY;
    initDefaultMoves();
}

/**
 * @brief Construct a new Champion::Values object
 * Uses Acid Rainbows as default values.
 */
Champion::Values::Values() :
    walk_speed(4.5), dash_speed(8.0), dash_start_speed(10.0), dash_turn_accel(1.5), dash_stop_deceleration(0.5), traction(0.7),
    max_air_speed(5.0), air_acceleration(0.3), air_friction(0), jump_speed(16), short_hop_speed(8), air_jump_speed(6), 
    ground_forward_jump_speed(3),ground_backward_jump_speed(3),air_forward_jump_speed(3),air_backward_jump_speed(3),
    gravity(DEFAULT_GRAVITY), max_fall_speed(0), fast_fall_speed(0),
    jump_squat_duration(5), dash_start_duration(-1), dash_stop_duration(-1), dash_turn_duration(-1), landing_duration(-1), guard_start_duration(0), guard_stop_duration(0), air_jumps(1)
{

}

/**
 * @brief Returns the internal name
 *
 * @return const std::string& internal string identifier.
 */

const std::string& Champion::getName(){
    return name;
}

void Champion::setDisplayName(const char* name){
    display_name.assign(name);
}

const std::string& Champion::getDisplayName() const {
    return display_name;
}

/**
 * @brief Adds a Move.
 * Uses in-place construction.
 * @param name the name that will be used as the map key.
 * @return Move* a pointer to the created Move.
 */
Move& Champion::addMove(const std::string& name){
    auto [node, success] = moves.try_emplace(name);

    if (!success) {
        throw KBFatal("Could not create move");
    }

    return node->second;
}

/**
 * @brief Returns a Move of this Champion.
 *
 * @param name the internal name (which is also the map key) of the wanted Animation.
 * @return Move* a pointer to the Move if the specified name is an existing key, NULL otherwise.
 */
const Move* Champion::getMove(const std::string& name) const{
    auto it = moves.find(name);
    if (it == moves.end()){
        return NULL;
    }
    return &(it->second);
}

/**
 * @brief Returns a Move of this Champion.
 *
 * @param name the internal name (which is also the map key) of the wanted Animation.
 * @return Move* a pointer to the Move if the specified name is an existing key, NULL otherwise.
 */
const Move* Champion::getMove(const char* name) const{
    auto it = moves.find(name);
    if (it == moves.end()){
        return NULL;
    }
    return &(it->second);
}

/**
 * @brief Returns a Move, or create it if it doesn't exist yet.
 * Unlike getMove, always returns a valid Animation. If the Animation didn't exist, it is \ref Move::Move() "default constructed".
 * @param name the name
 * @return Move&
 */

Move& Champion::tryMove(const char* name){
    auto [node, success] = moves.try_emplace(name);
    return node->second;
}

const Move* Champion::getDefaultMove(DefaultMoves move) const {
    return (default_moves[(int)move]);
}

/**
 * @brief Returns the Animation associated with a certain \ref Fighter#State "fighter state", if there is any.
 *
 * @param state
 * @return Animation* a pointer to the Animation. Can be NULL.
 */

const EntityAnimation* Champion::getDefaultAnimation(const DefaultAnimation anim) const {
    return default_animations[(int)anim];
}

void Champion::finalizeMoves(){
    const EntityAnimation* anim;
    for (auto& [name, move] : moves){
        anim = getAnimation(move.anim_name);
        if (anim){
            move.animation = anim;
        }
    }
}

/**
 * @brief Initializes the data relative to the currently added Animations of this Champion.
 * Initializes the state-animation association (based on the \ref Fighter#state_default_animation_name "state-animation name" association),
 * and the callbacks of certains animations based on their key in the map (e.g. the `jump` animation)
 */

void Champion::initDefaultAnimations(){
    EntityAnimation* anim;

    for (auto const& [state, name] : default_animation_name){
        if ((anim = (EntityAnimation*)getAnimation(name))){
            default_animations[(int)state] = anim;
            if (state == DefaultAnimation::JUMP) {
            }
        }
    }

    if ((anim = (EntityAnimation*)getDefaultAnimation(DefaultAnimation::JUMP))){
        anim->setNextAnimation(getDefaultAnimation(DefaultAnimation::AIR_IDLE));
        anim->setEndAction(Animation::EndAction::START_ANIMATION, getDefaultAnimation(DefaultAnimation::AIR_IDLE));
    }
    if ((anim = (EntityAnimation*)getDefaultAnimation(DefaultAnimation::AIR_JUMP))){
        anim->setNextAnimation(getDefaultAnimation(DefaultAnimation::AIR_IDLE));
    }
}

/**
 * @brief Creates all the moves that are supposed to be present on all champions
 * 
 */
void Champion::initDefaultMoves(){
    for (auto const& [moveID, name] : default_move_name){
        Move& move = addMove(name);
        move.anim_name = name;
        default_moves[(int)moveID] = &move;
    }
}

const std::map<Champion::DefaultAnimation, std::string> Champion::default_animation_name = {
    {Champion::DefaultAnimation::IDLE, "idle"},
    {Champion::DefaultAnimation::WALK, "walking"},
    {Champion::DefaultAnimation::WALK_TURN, "walk_turn"},
    {Champion::DefaultAnimation::JUMPSQUAT, "jumpsquat"},
    {Champion::DefaultAnimation::LANDING, "landing"},
    {Champion::DefaultAnimation::DASH, "dash"},
    {Champion::DefaultAnimation::DASH_START, "dash_start"},
    {Champion::DefaultAnimation::DASH_STOP, "dash_stop"},
    {Champion::DefaultAnimation::DASH_TURN, "dash_turn"},
    {Champion::DefaultAnimation::JUMP, "jump"},
    {Champion::DefaultAnimation::AIR_IDLE, "air_idle"},
    {Champion::DefaultAnimation::AIR_JUMP, "air_jump"},
    {Champion::DefaultAnimation::HITSTUN, "hurt"}
};

const std::map<Champion::DefaultMoves, std::string> Champion::default_move_name = {
    {Champion::DefaultMoves::Jab, "jab"},
    {Champion::DefaultMoves::Ftilt, "ftilt"},
    {Champion::DefaultMoves::UTilt, "utilt"},
    {Champion::DefaultMoves::DTilt, "dtilt"},
    {Champion::DefaultMoves::FSmash, "fsmash"},
    {Champion::DefaultMoves::USmash, "usmash"},
    {Champion::DefaultMoves::DSmash, "dsmash"},
    {Champion::DefaultMoves::Nair, "nair"},
    {Champion::DefaultMoves::Fair, "fair"},
    {Champion::DefaultMoves::BAir, "bair"},
    {Champion::DefaultMoves::UAir, "uair"},
    {Champion::DefaultMoves::DAir, "dair"},
    {Champion::DefaultMoves::NSpecial, "nspecial"},
    {Champion::DefaultMoves::SSpecial, "sspecial"},
    {Champion::DefaultMoves::USpecial, "uspecial"},
    {Champion::DefaultMoves::DSpecial, "dspecial"},

};