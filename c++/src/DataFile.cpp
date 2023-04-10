#include <string.h>
#include "SDL2/SDL_error.h"
#include "DataFile.h"
#include "AnimationsPool.h"
#include "EntityAnimation.h"
#include "Champion.h"
#include "StageModel.h"
#include "app.h"
#include "GameData.h"
#include "Util/util.h"
#include "Util/stringOperations.h"
#include "KBDebug/Debug.h"
#include "KBDebug/DebugTime.h"

#define FILE_SIGNATURE 0x54545454
#define FILEFORMAT_MAJOR 0
#define FILEFORMAT_MINOR 2
#define FILEFORMAT_REVISION 0
#include "fileMarkers.h"

constexpr std::string_view error_message = "File Loading : invalid data file content";
constexpr bool string_pointers_assumptions = true;

/**
 * @brief Reads data to the given buffer.
 * @tparam T data type. The size of this type is how much data is read.
 * @param res buffer
 */
template <typename T>
inline void DataFile::readData(T* res){
    SDL_RWread(sdl_stream, res, sizeof(T), 1);
}

/**
 * @brief Reads data to a buffer, writing the given default value instead if the data read
 * was max_value<T>
 * @tparam T data type. The size of this type is how much data is read.
 * @param res buffer
 * @param def value to use if the max value was read
 */
template <typename T>
inline void DataFile::readData(T* res, T def){
    readData(res, def, max_value<T>);
}

/**
 * @brief Reads data to a buffer, substituting a given value to another ; in other terms, if the value
 * read is A (the "indicator"), B (the "default value") is written instead
 * @tparam T data type. The size of this type is how much data is read.
 * @param res buffer
 * @param default value written if the value read was the indicator value
 * @param indicator if this value is read, the result is the default value instead
 */
template<typename T>
void DataFile::readData(T* res, T def, T ind){
    readData(res);
    if (*res == ind){
        *res = def;
    }
}

/**
 * @brief Reads data and returns it
 * @tparam T data type. The size of this type is how much data is read.
 * @return T data from the input stream.
 */
template<typename T>
inline T DataFile::readValue(){
    T val;
    DataFile::readData(&val);
    return val;
}

/**
 * @brief Reads data and returs it, or the given default value if the data read is the max_value
 * for this data type
 * @tparam T data type. The size of this type is how much data is read.
 * @param def default value, for if max_value<T> is read
 * @return T data from the input stream or def
 */
template<typename T>
inline T DataFile::readValue(T def){
    return readValue(def, max_value<T>);
}

template<typename T>
inline T DataFile::readValue(T def, T ind){
    T val = readValue<T>();
    return (val == ind) ? def : val;
}

int16_t DataFile::readWord(){
    int16_t word;
    DataFile::readData(&word);
    return word;
}

int16_t DataFile::readWord(int16_t& buffer){
    readData(&buffer);
    return buffer;
}

template<typename T>
inline bool isMax(T v){
    return v == max_value<T>;
}

/**
 * @brief Construct a new Data File object from a file path.
 * The file is directly opened.
 * @param filename
 */
DataFile::DataFile(const char* filename, SDL_Renderer* renderer_):
	sdl_stream(nullptr),
    string_read(readBuffer, 0),
    renderer(renderer_)
{
    file = fopen(filename, "rb");
    if (file){
        sdl_stream = SDL_RWFromFP(file, (SDL_bool)1);
    }
}

DataFile::~DataFile(){
	if (sdl_stream)	SDL_RWclose(sdl_stream);
}

/**
 * @brief Reads the first few bytes and check is their values were the Kuribrawl Data File Signature.
 * @return true If the signature confirmed that the file a Kuribrawl Data File
 * @return false otherwise.
 */

bool DataFile::checkSignature(){
    Uint32 buffer;
    SDL_RWread(sdl_stream, &buffer, 4, 1);
    return buffer == FILE_SIGNATURE;
}

/**
 * @brief Reads a bytes as the data file format version and prints it.
 *
 */
void DataFile::readVersion(){
    Uint8 buffer;
    Debug::out << "Version : ";
    SDL_RWread(sdl_stream, &buffer, 1, 1);
    Debug::out << (int)buffer << '.';
    SDL_RWread(sdl_stream, &buffer, 1, 1);
    Debug::out << (int)buffer << '.';
    SDL_RWread(sdl_stream, &buffer, 1, 1);
    Debug::out << (int)buffer << '\n' << std::flush;
}

/**
 * @brief Reads a \ref DataFile#DataType "DataType" from the file.
 *
 * @return DataFile::DataType
 */

DataFile::DataType DataFile::readDataType(){
    Uint8 buffer;
    if (!SDL_RWread(sdl_stream, &buffer, 1, 1)){
        return DataType::NONE;
    }
    return (DataType)buffer;
}

/**
 * @deprecated Use readString_ and get the result in string_read or readBuffer instead
 * @brief Reads a Data Chunk tag (name) from the file.
 * @return const char* the string identifier (tag or name) of a data chunk.
 */

char* DataFile::readFileTag(){
    readString();
    return readBuffer;
}

/**
 * @brief Returns whether the end of the file was reached
 */
bool DataFile::eof(){
    return feof(file);
}

/**
 * @brief Returns the current position in the file
 * @return long 
 */
long DataFile::tell(){
    return ftell(file);
}

void DataFile::readByteData(void* res){
    SDL_RWread(sdl_stream, res, 1, 1);
}

void DataFile::readWordData(void* res){
    SDL_RWread(sdl_stream, res, 2, 1);
}

void DataFile::readLongData (void* res){
    SDL_RWread(sdl_stream, res, 4, 1);
}

void DataFile::readDoubleData (void* res){
    SDL_RWread(sdl_stream, res, 8, 1);
}

/**
 * @brief Reads a c-string from the file.
 * Copies the string into the readBuffer array.
 */

/*
void DataFile::readString(){
    fgets(readBuffer, BUFFER_SIZE, file);
    readBuffer[strlen(readBuffer) - 1] = '\0';
}
*/

/**
 * @brief Prints the string read buffer to the Debug output (usually stdout)
 * in a human-readable-way (non-printable characters escaped as unicode code points)
 */
void DataFile::printStringBuffer() const {
    Debug::out.printReadable(readBuffer, BUFFER_SIZE);
    Debug::out.newline();
}

/**
 * @brief Reads a string from the file into the internal buffer of this Datafile
 * @return size_t number of characters read
 */
size_t DataFile::readString(){
    Debug::out << "Reading at 0x" << Logger::hex(tell()) << '/' << tell() << '\n';
    size_t count = 0;
    for (int c = getc(file); c != '\n' && count < BUFFER_SIZE && c < 256;){
        //Debug::out << (char)c  << ':' <<  c << '\n';
        readBuffer[count] = c;
        ++count;
        c = getc(file);
    }
    readBuffer[count] = '\0';  
    //printStringBuffer();
    return count;
}

/**
 * @brief Reads a string and updates the given string view to represent this string
 * 
 * @param sv The string view to update ; will represent the string read after this call.
 */
void DataFile::readString(Kuribrawl::string_view& sv){
    if constexpr(string_pointers_assumptions){
        sv.set(readString());
    } else {
        sv.set(readBuffer, readString());
    }
}

/**
 * @brief Reads a string and returns it as a new string view
 * 
 * @return Kuribrawl::string_view that represents the string read
 */
Kuribrawl::string_view DataFile::readString_sv(){
    return Kuribrawl::string_view(readBuffer, readString());
}

/**
 * @brief Reads a string and returns it as a C string (char*)
 * 
 * @return char* 
 */
char* DataFile::readString_get(){
    readString();
    return readBuffer;
}

/**
 * @brief Reads a string and sets the internal string view string_read to represent it.
 */
void DataFile::readString_(){
    string_read.set(readString());
}

/**
 * @brief Reads and updates the gameplay values of a champion.
 * @param champion 
 */
void DataFile::readChampionValues(Champion& champion){
    readData(&champion.values.walk_speed);
    readData(&champion.values.dash_speed);
    readData(&champion.values.dash_start_speed);
    readData(&champion.values.dash_turn_accel);
    readData(&champion.values.dash_stop_deceleration);
    readData(&champion.values.traction);
    readData(&champion.values.max_air_speed);
    readData(&champion.values.air_acceleration);
    readData(&champion.values.air_friction);
    readData(&champion.values.jump_speed);
    readData(&champion.values.short_hop_speed);
    readData(&champion.values.air_jump_speed);
    readData(&champion.values.ground_forward_jump_speed);
    readData(&champion.values.ground_backward_jump_speed);
    readData(&champion.values.air_forward_jump_speed);
    readData(&champion.values.air_backward_jump_speed);
    readData(&champion.values.gravity);
    readData(&champion.values.max_fall_speed);
    readData(&champion.values.fast_fall_speed);
    readData(&champion.values.weight);
    readData(&champion.values.jump_squat_duration);
    readData(&champion.values.dash_start_duration);
    readData(&champion.values.dash_stop_duration);
    readData(&champion.values.dash_turn_duration);
    readData(&champion.values.landing_duration);
    readData(&champion.values.guard_start_duration);
    readData(&champion.values.guard_stop_duration);
    readData(&champion.values.shield_info.size);
    readData(&champion.values.shield_info.x);
    readData(&champion.values.shield_info.y);
    readData(&champion.values.air_jumps);
}

/**
 * @brief Read a data chunk describing a Champion, updating an existing Champion object.
 * @param champion 
 */
void DataFile::readChampionFile(Champion& champion){
    readString();
    champion.setDisplayName(readBuffer);
    Debug::out << "Display name " << (champion.getDisplayName()) << '\n';

    readChampionValues(champion);

    Uint8 byte;
    Move* current_move = nullptr;
    bool leave_loop = false;

    do {
        readData(&byte);
        switch (byte){
            case FILEMARKER_MOVEINFO:
                readString();
                current_move = &champion.tryMove(readBuffer);
                Debug::out << "Move : " << readBuffer << '\n' << std::flush;
                break;
            case FILEMARKER_LANDINGLAG:
                if (!current_move){
                    throw KBFatalDetailed("Landing lag info present before any move info"s, error_message);
                }
                readData(&byte);
                current_move->landing_lag = byte;
                Debug::out << "Landing lag : " << (int)byte << '\n' << std::flush;
                break;
            case FILEMARKER_INTERFILE:
                Debug::log("Interfile");
                leave_loop = true;
                break;
            default:
                fseek(file, -1, SEEK_CUR);
                Debug::out << "Unexpected byte at 0x" << std::hex << (ftell(file)) << ", expected champion information type identifier, found " << (int)getc(file) << '\n';
                throw KBFatalDetailed(error_message, 
                    Kuribrawl::formatString("Unexpected byte at 0x%x, expected champion information type identifier, found %d\n", ftell(file), (int)getc(file)));
                break;
        }
    } while (!leave_loop);

}

/**
 * @brief Reads and updates the gameplay values of a Stage.
 * @param stage 
 */
void DataFile::readStageValues(StageModel& stage){
    stage.values.size.w = readWord();
    stage.values.size.h = readWord();
    stage.values.camera_bounds.x = readWord();
    stage.values.camera_bounds.y = readWord();
    stage.values.camera_bounds.w = readWord();
    stage.values.camera_bounds.h = readWord();
}

/**
 * @brief Reads data describing a platform and adds a platform based on these informations to the given Stage.
 * @param stage 
 * @return PlatformModel& the newly created platform.
 */
PlatformModel& DataFile::readPlatformData(StageModel& stage){
    int16_t x = readValue<int16_t>(0);
    int16_t y = readValue<int16_t>(0);
    int16_t w = readValue<int16_t>(0);
    return stage.addPlatform(w, x, y);
}

/**
 * @brief Reads data describing a stage beackground element and adds one to the given stage based on these informations.
 * @param stage 
 * @return StageBackgroundElement& the newly created background element.
 */
StageBackgroundElementModel& DataFile::readBackgroundElementData(StageModel& stage){
    readString_();
    StageBackgroundElementModel& element = stage.addBackgroundElement(string_read);

    int16_t x = readValue<int16_t>();

    if (isMax(x)){
        element.position.set(0, 0);
        element.depth = -1;
    } else {
        element.position.set(x, readValue<int16_t>());
        element.depth = readValue<int16_t>();
    }

    Debug::sout << "Background element :" << element.position.x << element.position.y << element.depth << '\n';
    return element;
}

/**
 * @brief Read a data chunk describing a StageModel, updating an existing StageModel object.
 * @param stage 
 */
void DataFile::readStageFile(StageModel& stage){
    readString();
    stage.setDisplayName(readBuffer);
    Debug::out << "Display name : " << (stage.getDisplayName()) << '\n';

    readStageValues(stage);

    Uint8 byte;
    PlatformModel* current_platform = nullptr;
    bool leave_loop = false;
    Kuribrawl::string_view animation_name;

    do {
        readData(&byte);
        switch (byte){
            case FILEMARKER_PLATFORMINFO:
                current_platform = &readPlatformData(stage);

                break;
            case FILEMARKER_BACKGROUNDELEMENT:
                Debug::out << "Reading background element at 0x" << Log::hex(tell()) << '\n';
                readBackgroundElementData(stage);
                
                for (const auto & [name, animation] : stage){
                    Debug::out << name << '\n';
                }

                break;
            case FILEMARKER_PLATFORMANIMATION:
                readString_();

                if (!current_platform){
                    throw KBFatalDetailed("Platform animation name info present before any platform info"s, error_message);
                }

                current_platform->animation = &stage.tryAnimation(string_read);

                break;
            case FILEMARKER_INTERFILE:
                Debug::log("Interfile");
                leave_loop = true;
                break;
        }
    } while (!leave_loop);
}

/**
 * @brief Reads an image as a SDL texture.
 * @return SDL_Texture* the newly created texture.
 */
SDL_Texture* DataFile::readTexture(){
    int fileEnd;
    readLongData(&fileEnd);

    #ifdef DEBUG
        Debug::out << "Reading texture at 0x" << std::hex << tell() << std::dec << 
        ", of size " << fileEnd << '\n';
    #endif

    fileEnd += ftell(file); //Value is the file adress right after the image

    SDL_Texture* result = IMG_LoadTexture_RW(renderer, sdl_stream, 0);
    if (result == nullptr){
        throw KBFatalDetailed(Kuribrawl::formatString("Could not read texture file : %s", IMG_GetError()), "Missing or corrupted file in data file");
    }
    fseek(file, fileEnd, SEEK_SET);
    return result;
}

/**
 * @brief Read a data fragment in an Animation data chunk, updating a given animation, based on the
 * previously read data marker.
 * @param anim the animation to update
 * @param marker the data marker found just before, indicating the type of data to be read now.
 * @param context persistent data
 * @return DataFile::DataReadingResult 
 */
DataFile::DataReadingResult DataFile::readAnimationData(AnimationBase& anim, Uint8 marker, DataFile::AnimationParsingData& context){
    switch (marker){
        case FILEMARKER_ANIMSPEED: {
            double valueD = readValue<double>();
            anim.setBaseSpeed(valueD);
        }
        break;
        
        case FILEMARKER_FRAMEINFO: {
            Debug::out << "Reading frame info at 0x" << Log::hex(tell()) << '\n';
            Uint8 byte = readValue<Uint8>();
            context.current_frame = anim.getFrame(byte);
            context.frame_id = byte;
        }
        return DataReadingResult::SET_FRAME;
        
        case FILEMARKER_FRAMEDURATION: {
            if (!context.current_frame){
                throw KBFatalDetailed("Found frame duration data info before any frame info", "Error in the data file");
            }
            int16_t word = readValue<int16_t>();
            context.current_frame->duration = word;
        }
        break;

        case FILEMARKER_FRAMEORIGIN: {
            if (!context.current_frame){
                throw KBFatalDetailed("Found frame duration data info before any frame info", "Error in the data file");
            }
            int16_t word = readValue<int16_t>();
            context.current_frame->origin.x = word;
            readData(&word);
            context.current_frame->origin.y = word;
            Debug::out << "Frame origin : " << context.current_frame->origin.x << " " << context.current_frame->origin.y << '\n' << std::flush;
        }
        break;
        case FILEMARKER_INTERFILE:
            Debug::log("Interfile");
            return DataReadingResult::LEAVE_lOOP;
        default:
            return DataReadingResult::NOTHING_DONE;        
    }

    return DataReadingResult::READ;
}

/**
            fseek(file, -1, SEEK_CUR);
            Debug::out << "Unexpected byte at 0x" << std::hex << (ftell(file)) << ", expected animation information type identifier, found " << (int)getc(file) << '\n';
            throw KBFatalDetailed("File Loading : invalid data file content",
                Kuribrawl::formatString("Unexpected byte at 0x%x, expected animation information type identifier, found %d", ftell(file), (int)getc(file)));

 */

bool DataFile::readEntityAnimationData(EntityAnimation& anim, Uint8 marker,DataFile::AnimationParsingData& anim_context, DataFile::EntityAnimationParsingData& context){
    DataReadingResult result = readAnimationData(anim, marker, anim_context);


    switch (result){
        case DataReadingResult::LEAVE_lOOP:
            return true;
        case DataReadingResult::READ:
            return false;
        case DataReadingResult::SET_FRAME:
            context.current_entity_frame = anim.getEntityFrame(anim_context.frame_id);
            return false;
    }

    switch (marker){
        case FILEMARKER_FRAMEMOVEMENT: {
            if (!context.current_entity_frame){
                throw KBFatalDetailed("Found frame duration data info before any frame info", "Error in the data file");
            }

            Uint8 byte; double valueD;

            readData(&byte);

            context.current_entity_frame->movement.x.enabled = byte & 1;
            context.current_entity_frame->movement.x.set_speed = getBit(byte, 1);
            context.current_entity_frame->movement.x.whole_frame = getBit(byte, 2);
            context.current_entity_frame->movement.y.enabled = getBit(byte, 3);
            context.current_entity_frame->movement.y.set_speed = getBit(byte, 4);
            context.current_entity_frame->movement.y.whole_frame = getBit(byte, 5);

            readData(&valueD);
            context.current_entity_frame->movement.x.value = valueD;
            readData(&valueD);
            context.current_entity_frame->movement.y.value = valueD;
            break;
        }
        case FILEMARKER_HURTBOXINFO: {
            Debug::out << "Reading hurtbox info at 0x" << Log::hex(tell()) << '\n';
            if (!context.current_entity_frame){
                throw KBFatalDetailed("Data file : found hurtbox info before any frame info", "Error in the data file");
            }
            int16_t word; Uint8 byte;
            Hurtbox& hurtbox = context.current_entity_frame->hurtboxes.emplace_back();
            
            readData(&word);
            hurtbox.x = word;
            if (hurtbox.x == max_value<short>){  
                hurtbox.x = -(anim_context.current_frame->origin.x);
                hurtbox.y =  (anim_context.current_frame->origin.y);
                hurtbox.w = anim_context.current_frame->display.w;
                hurtbox.h = anim_context.current_frame->display.h;
                hurtbox.type = Hurtbox::Type::NORMAL;
            } else {
                readData(&word);
                hurtbox.y = word;
                readData(&word);
                hurtbox.w = word;
                readData(&word);
                hurtbox.h = word;
                readData(&byte);
                hurtbox.type = (Hurtbox::Type)byte;
            }
        }
        break;
        case FILEMARKER_HITBOXINFO: { 
            if (!context.current_entity_frame){
                throw KBFatalDetailed("Data file : found hurtbox info before any frame info", "Error in the data file");
            }
            int16_t word; Uint8 byte; double valueD;

            Hitbox& hitbox = context.current_entity_frame->hitboxes.emplace_back();
            
            readData(&word);
            hitbox.x = word;
            readData(&word);
            hitbox.y = word;
            readData(&word);
            hitbox.w = word;
            readData(&word);
            hitbox.h = word;
            readData(&byte);
            hitbox.type = (Hitbox::Type)byte;

            Debug::out << "Hitbox : " << hitbox.x << " " << hitbox.y << " " << hitbox.w << " " << hitbox.h << '\n' << std::flush;

            switch (hitbox.type){
                case Hitbox::Type::DAMAGE:
                    readData(&valueD);
                    hitbox.damage = valueD;
                    readData(&word);
                    hitbox.angle = word;
                    readData(&valueD);
                    hitbox.base_knockback = valueD;
                    readData(&valueD);
                    hitbox.scalink_knockback = valueD;
                    readData(&byte);
                    hitbox.hit = byte;
                    readData(&byte);
                    hitbox.priority = byte;
                    break;
                default:
                    throw KBFatalDetailed("Unsupported or invalid hitbox type"s, error_message);
            }
        }
        break;
        default:
            Debug::out << "READ : Unexpected byte at 0x" << std::hex << (tell() - 1) << ", expected animation information type identifier, found " << (int)marker << '\n';
            throw KBFatalDetailed(
                Kuribrawl::formatString("Unexpected byte at 0x%x, expected animation information type identifier, found %d", (tell() - 1), (int)marker),
                error_message
            );
    }
    return false;
}

/**
 * @brief Reads an Animation Data Chunk
 *
 * @param anim the anim that was created or updated based on this Data Chunk
 */

void DataFile::readAnimationFile(AnimationBase& anim){
    bool leave_loop = false;
    AnimationParsingData anim_context;
    Debug::log("----Reading animation----");

    anim.setSpritesheet(readTexture());

    Uint8 byte = readValue<Uint8>();
    switch (byte){
        case FILEMARKER_INTERFILE:
            anim.initFrames(1);
            return;
        case FILEMARKER_DESCRIPTORSTART:
            Debug::out << "Reading animation descriptor at 0x" << Log::hex(tell()) << '\n';

            readData(&byte);
            anim.initFrames(byte);
            
            do {
                readData(&byte);
                DataReadingResult result = readAnimationData(anim, byte, anim_context);
                
                if (result == DataReadingResult::NOTHING_DONE){
                    fseek(file, -1, SEEK_CUR);
                    Debug::out << "Unexpected byte at 0x" << std::hex << (ftell(file)) << ", expected animation information type identifier, found " << (int)getc(file) << '\n';
                    throw KBFatalDetailed(
                        Kuribrawl::formatString("Unexpected byte at 0x%x, expected animation information type identifier, found %d", ftell(file), (int)getc(file)),
                        error_message
                    );

                }

                leave_loop = (result == DataReadingResult::LEAVE_lOOP);

            } while (!leave_loop);
            
            break;
        default:
            Debug::out << "Unexpected byte at 0x" << std::hex << (ftell(file) - 1) << " , expected 0xFF or 0xFE\n";
            throw KBFatalDetailed(error_message, 
                Kuribrawl::formatString("Unexpected byte at 0x%x, expected 0xFF or 0xFE", (ftell(file) - 1)));
    }
}


/**
 * @brief Reads an EntityAnimation Data Chunk
 *
 * @param anim the anim that was created or updated based on this Data Chunk
 */

void DataFile::readEntityAnimationFile(EntityAnimation& anim){
    //int value;
    bool leave_loop = false;
    EntityAnimationParsingData entity_anim_context;
    AnimationParsingData anim_context;
    Debug::log("----Reading animation----");

    anim.setSpritesheet(readTexture());

    Uint8 byte = readValue<Uint8>();
    switch (byte){
        case FILEMARKER_INTERFILE:
            anim.initFrames(1);
            return;
        case FILEMARKER_DESCRIPTORSTART:
            Debug::out << "Reading animation descriptor at 0x" << Log::hex(tell()) << '\n';

            readData(&byte);
            anim.initFrames(byte);
            
            do {
                readData(&byte);
                leave_loop = readEntityAnimationData(anim, byte, anim_context, entity_anim_context);
                
            } while (!leave_loop);
            
            break;
        default:
            Debug::out << "Unexpected byte at 0x" << std::hex << (ftell(file) - 1) << " , expected 0xFF or 0xFE\n";
            throw KBFatalDetailed(error_message, 
                Kuribrawl::formatString("Unexpected byte at 0x%x, expected 0xFF or 0xFE", (ftell(file) - 1)));
    }
}

void DataFile::readAnimation(GameData& gd){
    //tag = readFileTag();
    //entity = tag;
    //element = separateTag(tag);
    Uint8 prefix = readValue<Uint8>();
    readString_();

    Debug::out << "Prefix : " << (int)prefix << '\n';

    Kuribrawl::string_view entity(string_read);
    Kuribrawl::string_view element;

    separateTag(string_read, entity, element);
    Debug::out << "- Reading Animation  : " << entity << " / " << element << '\n';

    switch (prefix){
        case FILEMARKER_ANIMATIONPOOL_CHAMPION:
            readEntityAnimationFile(gd.tryChampion(entity).tryAnimation(element));
            break;
        case FILEMARKER_ANIMATIONPOOL_STAGE:
            readAnimationFile(gd.tryStage(entity).tryAnimation(element.data()));
            break;
        default:
            throw KBFatalDetailed("Unsupported animation tag prefix"s, error_message);
    }

    /*
    switch(entity[0]){
        default:
            readEntityAnimationFile(gd.tryChampion(entity).tryAnimation(element));
    }*/
}

/**
 * @brief Transforms a string containing a '/' into two strings.
 * The input string is now the first string.
 * @return const char* The second string.
 */
char* DataFile::separateTag(char* tag){
    char* res = strchr(tag, '/');
    if (!res){
        throw KBFatal("Data chunk tag doesn't contain a '/'");
    }

    *res = '\0';
    return res + 1;
}

/**
 * @brief Finds a separating '/' in a tag string, sets the 
 * If string_pointers_assumptions is set to true, assumes that left points on the same char as tag.
 *
 * @param tag full string to search in 
 * @param left will be set to the part before /
 * @param right will be set to the part after /
 */
void DataFile::separateTag(const Kuribrawl::string_view& tag, Kuribrawl::string_view& left, Kuribrawl::string_view& right){
    const char* res = tag.find('/');
    if (!res)
        throw KBFatal("Data chunk tag doesn't contain a '/'");

    ptrdiff_t diff = res - tag.cdata();
    left.set(diff);
    right.set(res + 1, tag.size() - diff - 1);

    if constexpr(!string_pointers_assumptions){
        left.set(tag.data());
    }

    //abcde/abcd
    // tag = 0, 10
    // left = 0, ??
    // right = ??, ??
    // res = 5
    // diff = 5
    // left = 0, 5 YES !
    // right = 6, 4
}

/**
 * @brief Reads the entirety of the file and populates or update the content of a GameData object based on the Data Chunks present in it.
 *
 * @param data the GameData that will be updated and populated.
 */

void DataFile::read(App& app){
    Debug::log("====== Reading data file ==============");

    DebugTime chrono;
    chrono.start();

    if (!checkSignature()) throw KBFatal("Couldn't open data file : wrong signature !");
    readVersion();
    //char* tag;
    //char *entity, *element;

    Kuribrawl::string_view tag(readBuffer, 0);
    //Kuribrawl::string_view entity(tag);
    //Kuribrawl::string_view element;

    while (!eof()){
        switch (readDataType()){
            case DataFile::DataType::ANIMATION:
                readAnimation(app.gameData());
                break;
            case DataFile::DataType::CHAMPION:
                tag = readFileTag();
                Debug::out << "- Reading Champions : " << tag << '\n';
                readChampionFile(app.gameData().tryChampion(tag));
                break;
            case DataFile::DataType::IMAGE:
                tag = readFileTag();
                Debug::out << "- Reading Image : " << tag << '\n';
                app.assets().textures.add(tag.data() , readTexture());
                break;
            case DataFile::DataType::STAGE:
                tag = readFileTag();
                Debug::out << "- Reading StageModel : " << tag << '\n';
                readStageFile(app.gameData().tryStage(tag));
                break;
            case DataFile::DataType::NONE:
                Debug::log("-None");
            default:
                break;
        }
    }
    Debug::out << "Loading time : " << chrono.endSec() << " secs.\n";
    Debug::log("=========== Data file loading finished ==============");

    app.gameData().checkData();
}

bool DataFile::ready(){
    return file != 0;
}