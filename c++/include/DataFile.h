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
 * Kuribrawl Data files are composed of an header and a sucession of independent **Data Chunks**.
 * Data Chunks can contain raw files (PNG, WAV, etc), and **Descriptors**, which are chunks of Kuribrawl-specific information (some Data Chunks contain only a raw file, or only a Descriptor).
 */

class DataFile {
    public:

    DataFile(const char* file);
    ~DataFile();

    void read(GameData& data);
    bool ready();

    private:

    /**
     * @brief The different type of Data Chunks a Data file can store
     * 
     */
    enum class DataType {
        ANIMATION, ///< An Animation source image + info data chunk.
        LEFTANIM, ///< An Animation \ref Animation#spritesheet_left "left image".
        CHAMPION, ///< Info about a Champion.
        STAGE, ///< Info about a Stage.
        IMAGE ///< A simple lone image.
    };

    //Reading
    bool eof();
    bool checkSignature(); 
    void readVersion();
    DataType readDataType();
    const char* readFileTag();
    SDL_Texture* LoadImageFromDatafile();

    void readAnimationFile(Animation& anim);

    FILE* file; ///< The underlying C-style File pointer that is used to read from the Data file.
    SDL_RWops* sdl_stream;  ///< An SDL stream created from the \ref DataFile#file "FILE*", to use SDL file-reading functions.
                            /**This structure will use the \ref DataFile#file "FILE*" as its underlying FILE*. */
    char readBuffer[BUFFER_SIZE];   ///< Buffer used to read strings.

    /**
     * @brief Values that can be interpreted as tags in the Data file.
     * These tags are used to know *where* we are in the file : if we are at the end of a certain chunk, or just before a certain information for example.
     */
    enum class FileMarker {
        DESCRIPTORSTART = 0x53, ///< Placed before the Descriptor in a Data Chunk.
        ANIMINFO = 0x1,         ///< In an Animation descriptor, placed before various info about the animation
        FRAMEINFO = 0x2,        ///< In an Animation descriptor, paced before a frame index, indicating that all Frame-related info will be about the corresponding Frame.
        FRAMEDURATION = 0x20,   ///< In an Animation descriptor, placed before the duration of a Frame.
        FRAMEORIGIN = 0x21,     ///< In an Animation descriptor, placed before the x and y coordinates of the \ref Frame#origin "origin" of a Frame.
        FRAMEMOVEMENT = 0x22,   ///< In an Animation descriptor, placed before data about how a Frame impacts the movement of an entity.
        HURTBOXINFO = 0x3,      ///< In an Animation descriptor, placed before a frame index and info about on of its Hurtboxes.
        HITBOXINFO = 0x4,       ///< In an Animation descriptor, placed before a frame index and info about on of its Hitboxes.
        MOVEINFO = 0x5,         ///< In a Champion descriptor, placed before the id of a move and info about it (land lag, ...)
        MULTIMOVE = 0x6,        ///< In a Champion descriptor, placed before the id of a move and info about its multimove properties.
        PLATFORMINFO = 0x7,     ///< In a Stage descriptor, placed before info about a platform.
        INTERFILE = 0x54        ///< Placed between Data Chunks.
    };
};