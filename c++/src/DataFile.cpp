#include <string.h>
#include "SDL2/SDL_error.h"
#include "DataFile.h"
#include "Debug.h"
#include "AnimationsPool.h"
#include "EntityAnimation.h"
#include "GameData.h"

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
	if (sdl_stream)	free(sdl_stream);
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
    SDL_RWread(sdl_stream, &buffer, 1, 1);
    return (DataType)buffer;
}

/**
 * @brief Reads a Data Chunk tag (name) from the file.
 *
 * @return const char* the string identifier (tag or name) of a data chunk.
 */

char* DataFile::readFileTag(){
    fgets(readBuffer, BUFFER_SIZE, file);
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
 * @brief Reads an Animation Data Chunk
 *
 * @param anim the anim that was created or updated based on this Data Chunk
 */

void DataFile::readEntityAnimationFile(EntityAnimation& anim){
    int value;
    Uint8 byte, current_frame_id;
    Uint16 word;
    double valueD;
    bool leave_loop;
    Frame* current_frame;
    EntityFrame* current_entity_frame = nullptr;
    Hurtbox* hurtbox = nullptr;
    Hitbox* hitbox = nullptr;


    readLong(&value);    //Image size

    long before = ftell(file);
    anim.setSpritesheet(IMG_LoadTexture_RW(renderer, sdl_stream, 0));
    fseek(file, 12, SEEK_CUR);
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
                        current_frame_id = byte;
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
                        current_entity_frame->movement_type.x = byte & 0b111;
                        current_entity_frame->movement_type.y = (byte & 0b111000) >> 3;
                        readData(&valueD);
                        current_entity_frame->movement.x = valueD;
                        readData(&valueD);
                        current_entity_frame->movement.y = valueD;
                        break;
                    case FILEMARKER_HURTBOXINFO:   
                        if (!current_entity_frame){
                            throw KBFatalDetailed("Data file : found hurtbox info before any frame info", "Error in the data file");
                        }
                        hurtbox = &(current_entity_frame->hurtboxes.emplace_back());
                        
                        readWord(&word);
                        hurtbox->x = word;
                        if (hurtbox->x == MAX_VALUE_SHORT){  
                            hurtbox->x = -(current_frame->origin.x);
                            hurtbox->y = -(current_frame->origin.y);
                            hurtbox->w = current_frame->display.w;
                            hurtbox->h = current_frame->display.h;
                        } else {
                            readWord(&word);
                            hurtbox->y = word;
                            readWord(&word);
                            hurtbox->w = word;
                            readWord(&word);
                            hurtbox->h = word;
                        }

                        readByte(&byte);
                        hurtbox->type = byte;
                        break;
                    case FILEMARKER_HITBOXINFO:      
                        hurtbox = &current_entity_frame->hurtboxes.emplace_back();
                        
                        readWord(&word);
                        hurtbox->x = word;
                        readWord(&word);
                        hurtbox->y = word;
                        readWord(&word);
                        hurtbox->w = word;
                        readWord(&word);
                        hurtbox->h = word;
                        readByte(&byte);
                        hurtbox->type = byte;
                        break;
                    case FILEMARKER_INTERFILE:
                        leave_loop = true;
                        break;
                    default:
                        cout << "Unexpected byte at 0x" << std::hex << (ftell(file) - 1) << ", expected animation information type identifier, found " << (int)getc(file) << '\n';
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

void DataFile::read(GameData& data){
    Debug::log("Reading data file ==============");
    if (!checkSignature()) throw KBFatal("Couldn't open data file : wrong signature !");
    readVersion();
    char* tag;
    char *entity, *element, *endChar;

    (void)entity; (void)element;

    while (!eof()){
        switch (readDataType()){
            case DataFile::DataType::ANIMATION:
                tag = readFileTag();
                entity = tag;
                element = separateTag(tag);
                Debug::log(entity);
                Debug::log(element);
                endChar = element + strlen(element) - 1;
                *endChar = 0;
                switch(entity[0]){
                    default:
                        readEntityAnimationFile(data.tryChampion(entity).tryAnimation(element));
                }
                
                return;
                break;
            default:
                break;
        }
    }
    Debug::log("Data file loading finished ==============");
}

bool DataFile::ready(){
    return file != 0;
}