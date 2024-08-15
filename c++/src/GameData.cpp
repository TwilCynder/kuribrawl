#include <memory>
#include "GameData.h"
#include "KBDebug/Debug.h"

/**
 * @brief Returns the Champion associated with the given name.
 *
 * @param name the string identifier of the wanted Champion
 * @return Champion* the Champion, or NULL if no Champion had this name.
 */
Champion* GameData::getChampion(const char* name){
    auto it = champions.find(name);
    if (it == champions.end()){
      return NULL;
    }
    return &(it->second);
}

/**
 * @brief Returns the Champion associated with the given name.
 *
 * @param name the string identifier of the wanted Champion
 * @return Champion* the Champion, or NULL if no Champion had this name.
 */
Champion* GameData::getChampion(const std::string& name){
    auto it = champions.find(name);
    if (it == champions.end()){
      return NULL;
    }
    return &(it->second);
}

/**
 * @brief Adds a new Champion to the GameData.
 * Contructs the Champion in-place.
 * @param name the name of the new Champion ; will be both its key in the GameData and its \ref Champion#name "name attribute".
 * @return Champion* the newly added Champion (can't be NULL since the method throws if it couldn't be created)
 */
Champion& GameData::addChampion(const std::string_view& name) {
    return addChampion((std::string)name);
}

Champion& GameData::addChampion(std::string&& name) {
    std::string name_copy = name; //string's gonna be copied at some point so i'm trying to control when
    auto [node, success] = champions.try_emplace(std::move(name), std::move(name_copy));
    if (!success) {
        throw KBFatal(Kuribrawl::formatString("Could not create champion %s", name));
    }

    Champion& champion = node->second;
    return champion;
}

/**
 * @brief Returns a Champion associated with the given name, creating it if there was none.
 *
 * @param name the name of the wanted Champion.
 * @return Champion& a reference to the Champion.
 */
Champion& GameData::tryChampion(const Kuribrawl::string_view& name){
    return tryChampion((std::string)name);
}

Champion& GameData::tryChampion(std::string&& name){
    auto [node, success] = champions.try_emplace(name, std::move(name)); 
    return node->second;
}

Champion& GameData::tryChampion(const std::string& name){
    auto [node, success] = champions.try_emplace(name, name); 
    return node->second;
}

/**
 * @brief Returns the Stage associated with the given name.
 *
 * @param name the string identifier of the wanted Stage
 * @return Stage* the Stage, or NULL if no Stage had this name.
 */
StageModel* GameData::getStage(const char* name){
    auto it = stages.find(name);
    if (it == stages.end()){
      return NULL;
    }
    return &(it->second);
}

/**
 * @brief Returns the Stage associated with the given name.
 *
 * @param name the string identifier of the wanted Stage
 * @return Stage* the Stage, or NULL if no Stage had this name.
 */
StageModel* GameData::getStage(const std::string& name){
    auto it = stages.find(name);
    if (it == stages.end()){
      return NULL;
    }
    return &(it->second);
}

/**
 * @brief Adds a new Stage to the GameData.
 * Contructs the Stage in-place.
 * @param name the name of the new Champion ; will be both its key in the GameData and its \ref Stage#name "name attribute".
 * @return Stage* the newly added Stage (can't be NULL since the method throws if it couldn't be created)
 */
StageModel& GameData::addStage(const std::string_view& name) {
    return addStage((std::string)name);
}

StageModel& GameData::addStage(std::string&& name) {
    std::string name_copy = name; //string's gonna be copied at some point so i'm trying to control when
    auto [node, success] = stages.try_emplace(std::move(name), std::move(name_copy));
    if (!success) {
        throw KBFatal(Kuribrawl::formatString("Could not create stage %s", name));
    }

    StageModel& stage = node->second;
    return stage;
}

/**
 * @brief Returns a Stage associated with the given name, creating it if there was none.
 *
 * @param name the name of the wanted Stage.
 * @return Stage& a reference to the Stage.
 */
StageModel& GameData::tryStage(const Kuribrawl::string_view& name){
    return tryStage((std::string)name);
}

StageModel& GameData::tryStage(std::string&& name){
    auto [node, success] = stages.try_emplace(name, std::move(name)); 
    return node->second;
}

StageModel& GameData::tryStage(const std::string& name){
    auto [node, success] = stages.try_emplace(name, name); 
    return node->second;
}

void GameData::finalizeChampionsInitialization(){
    ChampionsMap::iterator it;
    for (it = champions.begin(); it != champions.end(); it++)
    {
        it->second.initDefaultAnimations();
        it->second.finalizeMoves();
    }
}

void GameData::checkData() const {
    Debug::log("Verifying champions : ");
    for (const auto & [name, champion] : champions){
        Debug::out << "- " << name << '\n';
        for (const auto & [name, animation] : champion){
            Debug::out << "  - " << name << " : " << animation.getSpritesheet() << '\n';
            /*Debug::out << "  - - - end : " << (int)animation.gameplay_behavior.getEndBehavior() << '\n';
            for (auto window : animation.gameplay_behavior.getLandingBehavior()){
                GameplayAnimationBehavior::LandingBehaviorType type = window.behavior.type;
                Debug::sout << "  - - - Landing window : frame" << window.frame << "| type" << (int)type << '\n';
                if (type == GameplayAnimationBehavior::LandingBehaviorType::ANIMATION){
                    Debug::out << "  - - - - - Animation : " << window.behavior.animation.anim->getSpritesheet() << '\n';
                }
            }*/
        }
    }
    Debug::log("Verifying stages : ");
    for (const auto & [name, stage] : stages){
        Debug::out << "- " << name << '\n';
        for (const auto & [name, animation] : stage){
            Debug::out << "  - " << name << " : " << animation.getSpritesheet() << '\n';
        }
    }
}