#pragma once

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_rwops.h"
#include "GameData.h"
#include <cstdio>
#include <string>

#define BUFFER_SIZE 64

class Animation;

class DataFile {
    public:
    enum class FileType {
        ANIMATION,
        LEFTANIM,
        CHAMPION,
        STAGE,
        IMAGE
    };

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

    DataFile(const char* file);
    ~DataFile();

    void read(GameData& data);
    bool ready();

    private:

    //Reading
    bool eof(); ///
    bool checkSignature(); 
    void readVersion();
    FileType readFileType();
    const char* readFileTag();
    SDL_Texture* LoadImageFromDatafile();

    void readAnimationFile(Animation& anim);

    FILE* file;
    SDL_RWops* sdl_stream;
    char readBuffer[BUFFER_SIZE];
};