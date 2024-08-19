#include "Champion.h"
#include "KBDebug/Debug.h"
#include "Util/containers_util.h"

#define DEFAULT_GRAVITY 0.6

/**
 * POUR LES TRANSI ENTRE ANIMATIONS
 * On va commencer par juste faire un système de transitions animation -> animation
 * le truc de généraliser le concept du "si je veux jouer aur_idle après air_hurt en fait je joue air_idle_after_hurt" me paraît pas si utile
 * donc on va laisser le air_idle_after_hurt comme "cas spécial" et mettre le air_hurt_to_idle dans le nouveau système de transition
 */

/**
 * @brief Construct a new Champion:: Champion object
 *
 * @param name_ The internal name.
 */
Champion::Champion(std::string&& name_):
    name(std::move(name_)),
    default_animations(std::make_unique<const EntityAnimation*[]>((int)DefaultAnimation::TOTAL)),
    default_moves(std::make_unique<const Move* []>((int)DefaultMoves::TOTAL)),
    transition_matrix(),
    state_durations()
{
    initDefaultMoves();
}

/**
 * @brief Construct a new Champion:: Champion object
 *
 * @param name_ The internal name.
 */
Champion::Champion(const std::string& name_):
    Champion(std::string(name))
{
    initDefaultMoves();
}

/**
 * @brief Construct a new Champion:: Champion object
 *
 * @param name_ The internal name.
 */
Champion::Champion(const std::string_view& name_):
    Champion((std::string)name_)
{}

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

Move& Champion::addMove(std::string&& name){
    auto [node, success] = moves.try_emplace(std::move(name));

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

/**
 * @brief Returns a Move, or create it if it doesn't exist yet. The key here is a temporary string.
 * Unlike getMove, always returns a valid Animation. If the Animation didn't exist, it is \ref Move::Move() "default constructed".
 * @param name the name
 * @return Move&
 */
Move& Champion::tryMove(std::string&& name){
    auto [node, success] = moves.try_emplace(std::move(name));
    return node->second;
}

const Move* Champion::getDefaultMove(DefaultMoves move) const {
    return (default_moves[(int)move]);
}

duration_t Champion::getStateDuration(Kuribrawl::FighterState s) const{
    return state_durations[(int)s];
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

void Champion::setDefaultAnimation(const DefaultAnimation default_anim, const EntityAnimation* anim){
    if (default_anim >= DefaultAnimation::TOTAL) throw KBFatal("Wrong default_anim");
    default_animations[(int)default_anim] = anim;
}

const EntityAnimation* Champion::getAnimationTransition(const EntityAnimation& source, const EntityAnimation& destination) const {
    return transition_matrix.get(&source, &destination);
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

void Champion::finalize(){
    initDefaultAnimations();
    finalizeMoves();

    for (int i = 0; i < (int)Kuribrawl::FighterState::STATES; i++){
        state_durations[i] = -1;
    }
    state_durations[(int)Kuribrawl::FighterState::JUMPSQUAT] = values.jump_squat_duration;
    state_durations[(int)Kuribrawl::FighterState::DASH_START] = values.dash_start_duration;
    state_durations[(int)Kuribrawl::FighterState::DASH_TURN] = values.dash_turn_duration;
    state_durations[(int)Kuribrawl::FighterState::DASH_STOP] = values.dash_stop_duration;
    state_durations[(int)Kuribrawl::FighterState::LANDING] = values.landing_duration;
    state_durations[(int)Kuribrawl::FighterState::LANDING_LAG] = values.landing_duration;
    state_durations[(int)Kuribrawl::FighterState::GUARD_START] = values.guard_start_duration;
    state_durations[(int)Kuribrawl::FighterState::GUARD_STOP] = values.guard_stop_duration;
}

const EntityAnimation* Champion::resolveDefaultAnimation(Champion::DefaultAnimation id, std::unordered_set<Champion::DefaultAnimation>& already_seen){
    const EntityAnimation* anim = getDefaultAnimation(id);
    if (anim){
        return anim;
    } else {
        auto it = default_animations_fallbacks.find(id);
        if (it != default_animations_fallbacks.end()){
            DefaultAnimation fallback_id = it->second;

            already_seen.insert(id);

            if (already_seen.contains(fallback_id)){
                Debug::out << "WARNING : cyclic dependency found in optional default animations dependencies : animation " <<
                    Kuribrawl::getMapValue(default_animation_name, id) << " depends on animation " <<
                    Kuribrawl::getMapValue(default_animation_name, fallback_id) << '\n';
                return nullptr;
            }

            const EntityAnimation* fallback_anim = resolveDefaultAnimation(fallback_id, already_seen);
            Debug::out << "Using animation " << (int)fallback_id << " instead of " << (int)id << '\n';
            default_animations[(int)id] = fallback_anim;
            return fallback_anim;
        } else {
            return nullptr;
        }

    }
}

/**
 * @brief Initializes this Champions animation transition matrix, based on the default animation transitions.  
 * 
 */
void Champion::initAnimationTransitionMatrix(){
    const EntityAnimation *source, *destination, *transition;
    for (auto [source_id, dest_id, transition_name] : default_transition_animations_names.trange()){
        Debug::sout << (int)source_id << (int)dest_id << name << '\n';
        source = getDefaultAnimation(source_id);
        destination = getDefaultAnimation(dest_id);
        if (source && destination){
            transition = getAnimation(transition_name);
            if (transition)
                transition_matrix.try_emplace(source, destination, transition);
        }   
    }

    for (auto [s, d, t] : transition_matrix.trange()){
        Debug::sout << s->getSpritesheet() << d->getSpritesheet() << t->getSpritesheet() << '\n';
    }
}

/**
 * @brief Initializes the data relative to the currently added Animations of this Champion.
 * Initializes the state-animation association (based on the \ref Fighter#state_default_animation_name "state-animation name" association),
 * and the callbacks of certains animations based on their key in the map (e.g. the `jump` animation)
 */

void Champion::initDefaultAnimations(){
    EntityAnimation *anim;

    //Checking existing default animations
    for (auto const& [state, name] : default_animation_name){
        if ((anim = (EntityAnimation*)getAnimation(name))){
            default_animations[(int)state] = anim;
        }
    }

    //Checking optional default animations

    {
        std::unordered_set<DefaultAnimation> seen;
        for (auto const& [default_id, fallback_id] : default_animations_fallbacks){
            const EntityAnimation* anim = getDefaultAnimation(default_id);
            if (!anim){
                Debug::out << "We have no animation for ID " << (int)default_id << ", fallback id is " << (int)fallback_id << '\n';
                const EntityAnimation* fallback_anim = resolveDefaultAnimation(fallback_id, seen);
                seen.clear();
                Debug::out << "Using animation " << (int)fallback_id << " : " << (void*)fallback_anim << " instead of " << (int)default_id << '\n';
                default_animations[int(default_id)] = fallback_anim;
            }
        }
    }

    if ((anim = (EntityAnimation*)getDefaultAnimation(DefaultAnimation::JUMP))){
        anim->setEndAction(getDefaultAnimation(DefaultAnimation::AIR_IDLE));
    }
    if ((anim = (EntityAnimation*)getDefaultAnimation(DefaultAnimation::AIR_JUMP))){
        anim->setEndAction(getDefaultAnimation(DefaultAnimation::AIR_IDLE));
    }

    /*
    anim2 = (EntityAnimation*)getDefaultAnimation(DefaultAnimation::AIR_HITSTUN_TO_IDLE); 
    if (anim2){ //there is a air_hurt_to_idle animation
        anim2->setEndAction(anim);
    } else {
        setDefaultAnimation(DefaultAnimation::AIR_HITSTUN_TO_IDLE, anim);
    }
    */

    initAnimationTransitionMatrix();
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
    {Champion::DefaultAnimation::GUARD, "guard"},
    {Champion::DefaultAnimation::GUARD_START, "guard_start"},
    {Champion::DefaultAnimation::GUARD_STOP, "guard_stop"},
    {Champion::DefaultAnimation::WALK, "walking"},
    {Champion::DefaultAnimation::WALK_TURN, "walk_turn"},
    {Champion::DefaultAnimation::JUMPSQUAT, "jumpsquat"},
    {Champion::DefaultAnimation::LANDING, "landing"},
    {Champion::DefaultAnimation::DASH, "dash"},
    {Champion::DefaultAnimation::DASH_START, "dash_start"},
    {Champion::DefaultAnimation::DASH_STOP, "dash_stop"},
    {Champion::DefaultAnimation::DASH_TURN, "dash_turn"},
    {Champion::DefaultAnimation::JUMP, "jump"},
    {Champion::DefaultAnimation::JUMP_FORWARD, "jump_f"},
    {Champion::DefaultAnimation::JUMP_BACKWARD, "jump_b"},
    {Champion::DefaultAnimation::AIR_IDLE, "air_idle"},
    {Champion::DefaultAnimation::AIR_JUMP, "air_jump"},
    {Champion::DefaultAnimation::AIR_JUMP_FORWARD, "air_jump_f"},
    {Champion::DefaultAnimation::AIR_JUMP_BACKWARD, "air_jump_b"},
    {Champion::DefaultAnimation::HITSTUN, "hurt"},
    {Champion::DefaultAnimation::AIR_IDLE_AFTER_HIT, "air_idle_after_hurt"},
    {Champion::DefaultAnimation::FREEFALL, "freefall"}
    //a voir pour les animations tumble
    //pour le landing lag ce sera spécial aussi
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

const std::map<Champion::DefaultAnimation, Champion::DefaultAnimation> Champion::default_animations_fallbacks = {
    {Champion::DefaultAnimation::JUMP, Champion::DefaultAnimation::AIR_IDLE},
    {Champion::DefaultAnimation::AIR_JUMP, Champion::DefaultAnimation::JUMP},
    {Champion::DefaultAnimation::JUMP_FORWARD, Champion::DefaultAnimation::JUMP},
    {Champion::DefaultAnimation::JUMP_BACKWARD, Champion::DefaultAnimation::JUMP},
    {Champion::DefaultAnimation::AIR_JUMP_FORWARD, Champion::DefaultAnimation::AIR_JUMP},
    {Champion::DefaultAnimation::AIR_JUMP_BACKWARD, Champion::DefaultAnimation::AIR_JUMP},
    {Champion::DefaultAnimation::AIR_IDLE_AFTER_HIT, Champion::DefaultAnimation::AIR_IDLE},
};

const Kuribrawl::DynamicMatrixST<Champion::DefaultAnimation, std::string> Champion::default_transition_animations_names = {{
    {{Champion::DefaultAnimation::HITSTUN, Champion::DefaultAnimation::AIR_IDLE_AFTER_HIT}, "air_hurt_to_idle"},
}};