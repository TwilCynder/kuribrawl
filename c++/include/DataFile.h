#pragma once

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_rwops.h"
#include <cstdio>
#include <string>
#include "util/stringOperations.h"

#define BUFFER_SIZE 64

class EntityAnimation;
class Champion;
class Stage;
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
    long tell();

    void readByteData(void* res);
    void readWordData(void* res);
    void readLongData(void* res);
    void readDoubleData(void* res);

    //void readString();
    size_t readString();
    void readString(Kuribrawl::string_view&);
    Kuribrawl::string_view readString_sv();
    char*   readString_get();

    int16_t readWord();
    int16_t readWord(int16_t& buffer);

    bool checkSignature(); 
    void readVersion();
    DataType readDataType();
    char* readFileTag();

    //SDL-Specific
    SDL_Texture* readTexture();

    //Kuribrawl-Specific
    void readEntityAnimationFile(EntityAnimation& anim);
    void readChampionValues(Champion& champion);
    void readChampionFile(Champion& champion);
    void readStageValues(Stage& stage);
    void readStageFile(Stage& stage);

    template<typename T>
    void readData(T* res);

    static char* separateTag(char* tag);
    static void separateTag(const Kuribrawl::string_view& tag, Kuribrawl::string_view&, Kuribrawl::string_view&);

    FILE* file;             ///< The underlying C-style File pointer that is used to read from the Data file.
    SDL_RWops* sdl_stream;  ///< An SDL stream created from the \ref DataFile#file "FILE*", to use SDL file-reading functions.
                            /**This structure will use the \ref DataFile#file "FILE*" as its underlying FILE*. */
    char readBuffer[BUFFER_SIZE];   ///< Buffer used to read strings.
    SDL_Renderer* renderer; ///< The Renderer that will be used for textures (texture are bound to a renderer from creation so we need it when loading them)
};