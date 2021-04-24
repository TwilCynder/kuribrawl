#include <memory>
#include "load.h"
#include "Debug.h"

bool Load::loadGameFile(const char* name, GameData& data){
    DataFile data_file(name);
    if (data_file.ready()) {
        data_file.read(data);
        return true;
    }
    return false;

}