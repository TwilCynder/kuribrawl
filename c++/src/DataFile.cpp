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

constexpr bool string_pointers_assumptions = true;

template <typename T>
inline void DataFile::readData(T* res){
    SDL_RWread(sdl_stream, res, sizeof(T), 1);
}

template <typename T>
inline void DataFile::readData(T* res, T def){
    readData(res);
    if (*res == max_value<T>) 
        *res = def;
}

template<typename T>
inline T DataFile::readValue(){
    T val;
    DataFile::readData(&val);
    return val;
}

template<typename T>
inline T DataFile::readValue(T def){
    T val = readValue<T>();
    return (val == max_value<T>) ? def : val;
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

/**
 * @brief Construct a new Data File object from a file path.
 * The file is directly opened.
 * @param filename
 */
DataFile::DataFile(const char* filename, SDL_Renderer* renderer_):
	sdl_stream(nullptr),
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
 * @brief Reads a few bytes as the data file format version and prints it..
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
 * @brief Reads a Data Chunk tag (name) from the file.
 *
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

void DataFile::printStringBuffer() const {
    Debug::out.printReadable(readBuffer, BUFFER_SIZE);
    Debug::out.newline();
}

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

void DataFile::readString(Kuribrawl::string_view& sv){
    if constexpr(string_pointers_assumptions){
        sv.set(readString());
    } else {
        sv.set(readBuffer, readString());
    }
}

Kuribrawl::string_view DataFile::readString_sv(){
    return Kuribrawl::string_view(readBuffer, readString());
}

char* DataFile::readString_get(){
    readString();
    return readBuffer;
}

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
                    throw KBFatalDetailed("Landing lag info present before any move info", "File Loading : invalid data file content");
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
                throw KBFatalDetailed("File Loading : invalid data file content", 
                    Kuribrawl::formatString("Unexpected byte at 0x%x, expected champion information type identifier, found %d\n", ftell(file), (int)getc(file)));
                break;
        }
    } while (!leave_loop);

}

void DataFile::readStageValues(StageModel& stage){
    stage.values.size.w = readWord();
    stage.values.size.h = readWord();
    stage.values.camera_bounds.x = readWord();
    stage.values.camera_bounds.y = readWord();
    stage.values.camera_bounds.w = readWord();
    stage.values.camera_bounds.h = readWord();
}

PlatformModel& DataFile::readPlatformData(StageModel& stage){
    int16_t x = readValue<int16_t>(0);
    int16_t y = readValue<int16_t>(0);
    int16_t w = readValue<int16_t>(0);
    return stage.addPlatform(w, x, y);
}

void DataFile::readStageFile(StageModel& stage){
    readString();
    stage.setDisplayName(readBuffer);
    Debug::out << "Display name : " << (stage.getDisplayName()) << '\n';

    readStageValues(stage);

    Uint8 byte;
    PlatformModel* current_platform = nullptr;
    bool leave_loop = false;

    do {
        readData(&byte);
        switch (byte){
            case FILEMARKER_PLATFORMINFO:
                current_platform = &readPlatformData(stage);

                Debug::log(current_platform->pos.x);
                Debug::log(current_platform->pos.y);
                Debug::log(current_platform->w);

                break;
            case FILEMARKER_INTERFILE:
                Debug::log("Interfile");
                leave_loop = true;
                break;
        }
    } while (!leave_loop);
}

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
 * @brief Reads an Animation Data Chunk
 *
 * @param anim the anim that was created or updated based on this Data Chunk
 */

void DataFile::readEntityAnimationFile(EntityAnimation& anim){
    //int value;
    Uint8 byte;
    int16_t word;
    double valueD;
    bool leave_loop;
    Frame* current_frame;
    EntityFrame* current_entity_frame = nullptr;
    Hurtbox* hurtbox = nullptr;
    Hitbox* hitbox = nullptr;

    Debug::log("----Reading animation----");

    anim.setSpritesheet(readTexture());
    readData(&byte);
    switch (byte){
        case FILEMARKER_INTERFILE:
            anim.initFrames(1);
            return;
        case FILEMARKER_DESCRIPTORSTART:
            Debug::out << "Reading animation descriptor at 0x" << Log::hex(tell()) << '\n';

            readData(&byte);
            anim.initFrames(byte);
            
            leave_loop = false;
            do {
                readData(&byte);
                switch (byte){
                    case FILEMARKER_ANIMSPEED:
                        readData(&valueD);
                        anim.setBaseSpeed(valueD);
                        break;
                    case FILEMARKER_FRAMEINFO:
                        Debug::out << "Reading frame info at 0x" << Log::hex(tell()) << '\n';
                        readData(&byte);
                        current_frame = anim.getFrame(byte);
                        current_entity_frame = anim.getEntityFrame(byte);
                        break;
                    case FILEMARKER_FRAMEDURATION:
                        readData(&word);
                        current_frame->duration = word;
                        break;
                    case FILEMARKER_FRAMEORIGIN:
                        readWordData(&word);
                        current_frame->origin.x = word;
                        readWordData(&word);
                        current_frame->origin.y = word;
                        Debug::out << "Frame origin : " << current_frame->origin.x << " " << current_frame->origin.y << '\n' << std::flush;
                        break;
                    case FILEMARKER_FRAMEMOVEMENT:
                        readData(&byte);

                        current_entity_frame->movement.x.enabled = byte & 1;
                        current_entity_frame->movement.x.set_speed = getBit(byte, 1);
                        current_entity_frame->movement.x.whole_frame = getBit(byte, 2);
                        current_entity_frame->movement.y.enabled = getBit(byte, 3);
                        current_entity_frame->movement.y.set_speed = getBit(byte, 4);
                        current_entity_frame->movement.y.whole_frame = getBit(byte, 5);

                        readData(&valueD);
                        current_entity_frame->movement.x.value = valueD;
                        readData(&valueD);
                        current_entity_frame->movement.y.value = valueD;
                        break;
                    case FILEMARKER_HURTBOXINFO:
                        Debug::out << "Reading hurtbox info at 0x" << Log::hex(tell()) << '\n';
                        if (!current_entity_frame){
                            throw KBFatalDetailed("Data file : found hurtbox info before any frame info", "Error in the data file");
                        }
                        hurtbox = &(current_entity_frame->hurtboxes.emplace_back());
                        
                        readData(&word);
                        hurtbox->x = word;
                        if (hurtbox->x == MAX_VALUE_SHORT){  
                            hurtbox->x = -(current_frame->origin.x);
                            hurtbox->y =  (current_frame->origin.y);
                            hurtbox->w = current_frame->display.w;
                            hurtbox->h = current_frame->display.h;
                            hurtbox->type = Hurtbox::Type::NORMAL;
                        } else {
                            readData(&word);
                            hurtbox->y = word;
                            readData(&word);
                            hurtbox->w = word;
                            readData(&word);
                            hurtbox->h = word;
                            readData(&byte);
                            hurtbox->type = (Hurtbox::Type)byte;
                        }

                        break;
                    case FILEMARKER_HITBOXINFO:      
                        hitbox = &current_entity_frame->hitboxes.emplace_back();
                        
                        readData(&word);
                        hitbox->x = word;
                        readData(&word);
                        hitbox->y = word;
                        readData(&word);
                        hitbox->w = word;
                        readData(&word);
                        hitbox->h = word;
                        readData(&byte);
                        hitbox->type = (Hitbox::Type)byte;

                        Debug::out << "Hitbox : " << hitbox->x << " " << hitbox->y << " " << hitbox->w << " " << hitbox->h << '\n' << std::flush;

                        switch (hitbox->type){
                            case Hitbox::Type::DAMAGE:
                                readData(&valueD);
                                hitbox->damage = valueD;
                                readData(&word);
                                hitbox->angle = word;
                                readData(&valueD);
                                hitbox->base_knockback = valueD;
                                readData(&valueD);
                                hitbox->scalink_knockback = valueD;
                                readData(&byte);
                                hitbox->hit = byte;
                                readData(&byte);
                                hitbox->priority = byte;
                                break;
                            default:
                                throw KBFatalDetailed("Unsupported or invalid hitbox type", "File Loading : invalid data file content");
                        }

                        break;
                    case FILEMARKER_INTERFILE:
                        Debug::log("Interfile");
                        leave_loop = true;
                        break;
                    default:
                        fseek(file, -1, SEEK_CUR);
                        Debug::out << "Unexpected byte at 0x" << std::hex << (ftell(file)) << ", expected animation information type identifier, found " << (int)getc(file) << '\n';
                        throw KBFatalDetailed("File Loading : invalid data file content",
                            Kuribrawl::formatString("Unexpected byte at 0x%x, expected animation information type identifier, found %d", ftell(file), (int)getc(file)));
                }
            } while (!leave_loop);
            
            break;
        default:
            Debug::out << "Unexpected byte at 0x" << std::hex << (ftell(file) - 1) << " , expected 0xFF or 0xFE\n";
            throw KBFatalDetailed("File Loading : invalid data file content", 
                Kuribrawl::formatString("Unexpected byte at 0x%x, expected 0xFF or 0xFE", (ftell(file) - 1)));
    }
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
    Kuribrawl::string_view entity(tag);
    Kuribrawl::string_view element;

    while (!eof()){
        switch (readDataType()){
            case DataFile::DataType::ANIMATION:

                //tag = readFileTag();
                readString(tag);
                //entity = tag;
                //element = separateTag(tag);
                separateTag(tag, entity, element);
                Debug::log(entity.length());
                Debug::out << "- Reading Animation  : " << entity << " / " << element << '\n';

                switch(entity[0]){
                    default:
                        readEntityAnimationFile(app.gameData().tryChampion(entity).tryAnimation(element.data()));
                }
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
}

bool DataFile::ready(){
    return file != 0;
}