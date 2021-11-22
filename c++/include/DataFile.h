#pragma once

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_rwops.h"
#include <cstdio>
#include <string>

#define BUFFER_SIZE 64

class EntityAnimation;
class Champion;
class App;

/**
 * @brief An opened file, treated as a Kuribrawl Data File.
 * Kuribrawl Data files are composed of an header and a sucession of independent **Data Chunks**.
 * Data Chunks can contain raw files (PNG, WAV, etc), and **Descriptors**, which are chunks of Kuribrawl-specific information (some Data Chunks contain only a raw file, or only a Descriptor).
 */

class DataFile {
    public:

    DataFile(const char* file, SDL_Renderer* renderer);
    ~DataFile();

    void read(App& data);
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
        IMAGE, ///< A simple lone image.
        NONE   ///< Used when eof was read instead of a data type
    };

    //Reading
    bool eof();
    bool checkSignature(); 
    void readVersion();
    DataType readDataType();
    char* readFileTag();
    char* separateTag(char* tag);
    SDL_Texture* LoadImageFromDatafile();

    void readEntityAnimationFile(EntityAnimation& anim);
    void readChampionValues(Champion& champion);
    void readChampionFile(Champion& champion);

    void readByte(void* res);
    void readWord(void* res);
    void readLong(void* res);
    void readDouble(void* res);
    void readString();

    template<typename T>
    void readData(T* res);

    FILE* file;             ///< The underlying C-style File pointer that is used to read from the Data file.
    SDL_RWops* sdl_stream;  ///< An SDL stream created from the \ref DataFile#file "FILE*", to use SDL file-reading functions.
                            /**This structure will use the \ref DataFile#file "FILE*" as its underlying FILE*. */
    char readBuffer[BUFFER_SIZE];   ///< Buffer used to read strings.
    SDL_Renderer* renderer; ///< The Renderer that will be used for textures (texture are bound to a renderer from creation so we need it when loading them)
};