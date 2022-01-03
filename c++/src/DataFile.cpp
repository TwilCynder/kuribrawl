#include <string.h>
#include "SDL2/SDL_error.h"
#include "DataFile.h"
#include "KBDebug/Debug.h"
#include "AnimationsPool.h"
#include "EntityAnimation.h"
#include "Champion.h"
#include "app.h"
#include "GameData.h"
#include "Util/util.h"

#define FILE_SIGNATURE 0x54545454
#include "fileMarkers.h"

template <typename T>
void DataFile::readData(T* res){
    SDL_RWread(sdl_stream, res, sizeof(T), 1);
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
    cout << "Version : ";
    SDL_RWread(sdl_stream, &buffer, 1, 1);
    cout << (int)buffer << '.';
    SDL_RWread(sdl_stream, &buffer, 1, 1);
    cout << (int)buffer << '.';
    SDL_RWread(sdl_stream, &buffer, 1, 1);
    cout << (int)buffer << '\n' << std::flush;
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

void DataFile::readByte(void* res){
    SDL_RWread(sdl_stream, res, 1, 1);
}

void DataFile::readWord(void* res){
    SDL_RWread(sdl_stream, res, 2, 1);
}

void DataFile::readLong (void* res){
    SDL_RWread(sdl_stream, res, 4, 1);
}

void DataFile::readDouble (void* res){
    SDL_RWread(sdl_stream, res, 8, 1);
}

/**
 * @brief Reads a c-string from the file.
 * Copies the string into the readBuffer array.
 */
void DataFile::readString(){
    fgets(readBuffer, BUFFER_SIZE, file);
    readBuffer[strlen(readBuffer) - 1] = '\0';
}

void DataFile::readChampionValues(Champion& champion){
    readData(&champion.val.walk_speed);
    readData(&champion.val.dash_speed);
    readData(&champion.val.dash_start_speed);
    readData(&champion.val.dash_turn_accel);
    readData(&champion.val.dash_stop_deceleration);
    readData(&champion.val.traction);
    readData(&champion.val.max_air_speed);
    readData(&champion.val.air_acceleration);
    readData(&champion.val.air_friction);
    readData(&champion.val.jump_speed);
    readData(&champion.val.short_hop_speed);
    readData(&champion.val.air_jump_speed);
    readData(&champion.val.ground_forward_jump_speed);
    readData(&champion.val.ground_backward_jump_speed);
    readData(&champion.val.air_forward_jump_speed);
    readData(&champion.val.air_backward_jump_speed);
    readData(&champion.val.gravity);
    readData(&champion.val.max_fall_speed);
    readData(&champion.val.fast_fall_speed);
    readData(&champion.val.weight);
    readData(&champion.val.jump_squat_duration);
    readData(&champion.val.dash_start_duration);
    readData(&champion.val.dash_stop_duration);
    readData(&champion.val.dash_turn_duration);
    readData(&champion.val.landing_duration);
    readData(&champion.val.guard_start_duration);
    readData(&champion.val.guard_stop_duration);
    readData(&champion.val.shield_info.size);
    readData(&champion.val.shield_info.x);
    readData(&champion.val.shield_info.y);
    readData(&champion.val.air_jumps);
}

void DataFile::readChampionFile(Champion& champion){
    readString();
    champion.setDisplayName(readBuffer);
    cout << "Display name " << (champion.getDisplayName()) << '\n';

    readChampionValues(champion);

    Uint8 byte;
    Move* current_move = nullptr;
    bool leave_loop = false;

    do {
        readByte(&byte);
        switch (byte){
            case FILEMARKER_MOVEINFO:
                readString();
                current_move = &champion.tryMove(readBuffer);
                cout << "Move : " << readBuffer << '\n' << std::flush;
                break;
            case FILEMARKER_LANDINGLAG:
                if (!current_move){
                    throw KBFatalDetailed("File read : invalid data file content", "Landing lag info present before any move info");
                }
                readByte(&byte);
                current_move->landing_lag = byte;
                cout << "Landing lag : " << (int)byte << '\n' << std::flush;
                break;
            case FILEMARKER_INTERFILE:
                Debug::log("Interfile");
                leave_loop = true;
                break;
            default:
                fseek(file, -1, SEEK_CUR);
                cout << "Unexpected byte at 0x" << std::hex << (ftell(file)) << ", expected champion information type identifier, found " << (int)getc(file) << '\n';
                throw KBFatalExplicit("File read : invalid data file content");
                break;
        }
    } while (!leave_loop);

}

/**
 * @brief Reads an Animation Data Chunk
 *
 * @param anim the anim that was created or updated based on this Data Chunk
 */

void DataFile::readEntityAnimationFile(EntityAnimation& anim){
    int value;
    Uint8 byte;
    int16_t word;
    double valueD;
    bool leave_loop;
    Frame* current_frame;
    EntityFrame* current_entity_frame = nullptr;
    Hurtbox* hurtbox = nullptr;
    Hitbox* hitbox = nullptr;


    readLong(&value);    //Image size
    value += ftell(file); //Value is the file adress right after the image

    anim.setSpritesheet(IMG_LoadTexture_RW(renderer, sdl_stream, 0));
    fseek(file, value, SEEK_SET);
    readByte(&byte);
    switch (byte){
        case FILEMARKER_INTERFILE:
            return;
        case FILEMARKER_DESCRIPTORSTART:
            readByte(&byte);
            anim.initFrames(byte);
            
            leave_loop = false;
            do {
                readByte(&byte);
                switch (byte){
                    case FILEMARKER_ANIMSPEED:
                        readData(&valueD);
                        anim.setBaseSpeed(valueD);
                        break;
                    case FILEMARKER_FRAMEINFO:
                        readByte(&byte);
                        current_frame = anim.getFrame(byte);
                        current_entity_frame = anim.getEntityFrame(byte);
                        break;
                    case FILEMARKER_FRAMEDURATION:
                        readWord(&word);
                        current_frame->duration = word;
                        break;
                    case FILEMARKER_FRAMEORIGIN:
                        readLong(&value);
                        current_frame->origin.x = value;
                        readLong(&value);
                        current_frame->origin.x = value;
                        break;
                    case FILEMARKER_FRAMEMOVEMENT:
                        readByte(&byte);

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
                        if (!current_entity_frame){
                            throw KBFatalDetailed("Data file : found hurtbox info before any frame info", "Error in the data file");
                        }
                        hurtbox = &(current_entity_frame->hurtboxes.emplace_back());
                        
                        readWord(&word);
                        hurtbox->x = word;
                        if (hurtbox->x == MAX_VALUE_USHORT){  
                            hurtbox->x = -(current_frame->origin.x);
                            hurtbox->y =  (current_frame->origin.y);
                            hurtbox->w = current_frame->display.w;
                            hurtbox->h = current_frame->display.h;
                            hurtbox->type = Hurtbox::Type::NORMAL;
                        } else {
                            readWord(&word);
                            hurtbox->y = word;
                            readWord(&word);
                            hurtbox->w = word;
                            readWord(&word);
                            hurtbox->h = word;
                            readByte(&byte);
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

                        cout << "Hitbox : " << hitbox->x << " " << hitbox->y << " " << hitbox->w << " " << hitbox->h << '\n' << std::flush;

                        switch (hitbox->type){
                            case Hitbox::Type::DAMAGE:
                                readData(&valueD);
                                hitbox->damage = valueD;
                                readData(&word);
                                hitbox->angle = value;
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
                                throw KBFatalDetailed("File read : invalid data file content", "Unsupported or invalid hitbox type");
                        }

                        break;
                    case FILEMARKER_INTERFILE:
                        Debug::log("Interfile");
                        leave_loop = true;
                        break;
                    default:
                        fseek(file, -1, SEEK_CUR);
                        cout << "Unexpected byte at 0x" << std::hex << (ftell(file)) << ", expected animation information type identifier, found " << (int)getc(file) << '\n';
                        throw KBFatalExplicit("File read : invalid data file content");
                }
            } while (!leave_loop);
            
            break;
        default:
            cout << "Unexpected byte at 0x" << std::hex << (ftell(file) - 1) << " , expected 0xFF or 0xFE\n";
            throw KBFatalExplicit("File read : invalid data file content");
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
        cout << "At " << std::hex << ftell(file) << '\n';
        throw KBFatal("Data chunk tag doesn't contain a '/'");
    }

    *res = '\0';
    return res + 1;
}

/**
 * @brief Reads the entirety of the file and populates or update the content of a GameData object based on the Data Chunks present in it.
 *
 * @param data the GameData that will be updated and populated.
 */

void DataFile::read(App& app){
    Debug::log("====== Reading data file ==============");
    if (!checkSignature()) throw KBFatal("Couldn't open data file : wrong signature !");
    readVersion();
    char* tag;
    char *entity, *element;

    while (!eof()){
        switch (readDataType()){
            case DataFile::DataType::ANIMATION:

                tag = readFileTag();
                entity = tag;
                element = separateTag(tag);
                Debug::log("-Reading animation");
                Debug::log(entity);
                Debug::log(element);

                switch(entity[0]){
                    default:
                        readEntityAnimationFile(app.gameData().tryChampion(entity).tryAnimation(element));
                }
                break;
            case DataFile::DataType::CHAMPION:
                tag = readFileTag();
                Debug::log("-Reading CHampion");
                Debug::log(tag);
                readChampionFile(app.gameData().tryChampion(tag));
                break;
            case DataFile::DataType::IMAGE:

            case DataFile::DataType::NONE:
                Debug::log("-None");
            default:
                break;
        }
    }
    Debug::log("====== Data file loading finished ==============");
}

bool DataFile::ready(){
    return file != 0;
}