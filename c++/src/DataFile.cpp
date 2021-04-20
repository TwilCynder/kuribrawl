#include "SDL2/SDL_error.h"
#include "DataFile.h"
#include "Debug.h"
#include "Animation.h"
#include "GameData.h"

#define FILE_SIGNATURE 0x54545454

/**
 * @brief Construct a new Data File object from a file path.
 * The file is directly opened.
 * @param filename 
 */
DataFile::DataFile(const char* filename){
    file = fopen(filename, "r");
    if (file){
        sdl_stream = SDL_RWFromFP(file, (SDL_bool)1);
    }
}

DataFile::~DataFile(){
    free(sdl_stream);
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

const char* DataFile::readFileTag(){
    fgets(readBuffer, BUFFER_SIZE, file);
    return readBuffer;
}

/**
 * @brief Returns whether the end of the file was reached
 */

bool DataFile::eof(){
    return feof(file);
}

/**
 * @brief Reads an Animation Data Chunk
 * 
 * @param anim the anim that was created or updated based on this Data Chunk
 */

void DataFile::readAnimationFile(Animation& anim){

}

/**
 * @brief Reads the entirety of the file and populates or update the content of a GameData object based on the Data Chunks present in it.
 * 
 * @param data the GameData that will be updated and populated.
 */

void DataFile::read(GameData& data){
    Debug::log("Reading data file==============");
    if (!checkSignature()) throw KBFatal("Couldn't open data file : wrong signature !");

    readVersion();

    const char *tag, *entity, *element;

    (void)entity; (void)element;

    while (!eof()){
        switch (readDataType()){
            case DataFile::DataType::ANIMATION:
                
                //readAnimationFile(data.tryChampion(tag).tryAnimation(tag));

                tag = readFileTag();
                Debug::log(tag);
                return;
                break;
            default:
                break;
        }
    }
}  

bool DataFile::ready(){
    return file != 0;
}