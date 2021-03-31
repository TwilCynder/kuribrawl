#include <memory>
#include "GameData.h"
#include "DataFile.h"
#include "Debug.h"

Champion* GameData::getChampion(const std::string& name){
    auto it = champions.find(name);
    if (it == champions.end()){
      return NULL;
    }
    return &(it->second);
}

Champion* GameData::addChampion(const std::string& name) {
    auto [node, success] = champions.try_emplace(name, name);
    if (!success) {
        throw KBFatal("Could not create champion");
    }

    Champion& champion = node->second;
    return &champion;
}

Champion& GameData::tryChampion(const std::string& name){
    auto [node, success] = champions.try_emplace(name, name);
    return node->second;
}

void GameData::readDataFile(){
    DataFile data_file = DataFile("../src/res/data.twl");

    if (!data_file.checkSignature()) throw KBFatal("Couldn't open data file : wrong signature !");

    data_file.readVersion();

    std::string tag, entity, name;

    while (!data_file.eof()){
        switch (data_file.readFileType()){
            case DataFile::FileType::ANIMATION:
                tag.assign(data_file.readFileTag());
                Debug::log(tag);
                return;
                break;
            default:
                break;
        }
    }
}  