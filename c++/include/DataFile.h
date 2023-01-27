#pragma once

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_rwops.h"
#include <cstdio>
#include <string>
#include "util/stringOperations.h"

#define BUFFER_SIZE 64

class App;
class GameData;
class EntityAnimation;
class Animation;
class Champion;
class StageModel;
class StageBackgroundElementModel;
class PlatformModel;
class Frame;
class EntityFrame;
class Hitbox;
class Hurtbox;

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

    struct AnimationParsingData {
        Frame* current_frame = nullptr;
        int frame_id = 0;
    };

    struct EntityAnimationParsingData {
        EntityFrame* current_entity_frame = nullptr;
        Hurtbox* hurtbox = nullptr;
        Hitbox* hitbox = nullptr;
    };

    private:

    /**
     * @brief The different type of Data Chunks a Data file can store
     * 
     */
    enum class DataType {
        ANIMATION, ///< An Animation source image + info data chunk.
        LEFTANIM, ///< An Animation \ref Animation#spritesheet_left "left image".
        CHAMPION, ///< Info about a Champion.
        STAGE, ///< Info about a StageModel.
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
    void readString_();

    int16_t readWord();
    int16_t readWord(int16_t& buffer);

    //SDL-Specific
    SDL_Texture* readTexture();

    //Kuribrawl-Specific
    bool checkSignature(); 
    void readVersion();
    DataType readDataType();
    char* readFileTag();
    void readAnimation(GameData&);
    void readAnimationFile(Animation& anim);
    void readEntityAnimationFile(EntityAnimation& anim);
    void readChampionValues(Champion& champion);
    void readChampionFile(Champion& champion);
    void readStageValues(StageModel& stage);
    void readStageFile(StageModel& stage);
    PlatformModel& readPlatformData(StageModel& stage);
    StageBackgroundElementModel& readBackgroundElementData(StageModel& stage);

    enum class DataReadingResult {
        READ, NOTHING_DONE, LEAVE_lOOP, SET_FRAME
    };

    DataReadingResult readAnimationData(Animation&, Uint8 marker, AnimationParsingData& );
    bool readEntityAnimationData(EntityAnimation&, Uint8 marker, AnimationParsingData&, EntityAnimationParsingData& );

    template<typename T>
    void readData(T* res);
    template<typename T>
    void readData(T* res, T default_);
    template<typename T>
    void readData(T* res, T default_, T indicator);
    template<typename T>
    T readValue();
    template<typename T>
    T readValue(T default_);
    template<typename T>
    T readValue(T default_, T indicator);

    static char* separateTag(char* tag);
    static void separateTag(const Kuribrawl::string_view& tag, Kuribrawl::string_view&, Kuribrawl::string_view&);

    //Debug
    void printStringBuffer() const;

    FILE* file;             ///< The underlying C-style File pointer that is used to read from the Data file.
    SDL_RWops* sdl_stream;  ///< An SDL stream created from the \ref DataFile#file "FILE*", to use SDL file-reading functions.
                            /**This structure will use the \ref DataFile#file "FILE*" as its underlying FILE*. */
    char readBuffer[BUFFER_SIZE];   ///< Buffer used to read strings.
    Kuribrawl::string_view string_read;
    SDL_Renderer* renderer; ///< The Renderer that will be used for textures (texture are bound to a renderer from creation so we need it when loading them)
};