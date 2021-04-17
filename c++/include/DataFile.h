#pragma once

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_rwops.h"
#include <cstdio>
#include <string>

#define BUFFER_SIZE 64

class Animation;
class GameData;

/**
 * @brief An opened file, treated as a Kuribrawl Data File.
 */

class DataFile {
    public:

    DataFile(const char* file);
    ~DataFile();

    void read(GameData& data);
    bool ready();

    private:

    /**
     * @brief The different type of data a Data file can store
     * 
     */
    enum class DataType {
        ANIMATION, ///< aaa 
        LEFTANIM,
        CHAMPION,
        STAGE,
        IMAGE
    };

    //Reading
    bool eof(); ///
    bool checkSignature(); 
    void readVersion();
    DataType readDataType();
    const char* readFileTag();
    SDL_Texture* LoadImageFromDatafile();

    void readAnimationFile(Animation& anim);

    FILE* file;
    SDL_RWops* sdl_stream;
    char readBuffer[BUFFER_SIZE];

    enum class FileMarker {
        DESCRIPTORSTART = 0x53,
        ANIMINFO = 0x1,
        FRAMEINFO = 0x2,
        FRAMEDURATION = 0x20,
        FRAMEORIGIN = 0x21,
        FRAMEMOVEMENT = 0x22,
        HURTBOXINFO = 0x3,
        HITBOXINFO = 0x4,
        MOVEINFO = 0x5,
        MULTIMOVE = 0x6,
        PLATFORMINFO = 0x7,
        INTERFILE = 0x54
    };
};