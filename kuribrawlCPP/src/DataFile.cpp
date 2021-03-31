#include "SDL2/SDL_error.h"
#include "DataFile.h"
#include "Debug.h"

#define FILE_SIGNATURE 0x54545454

DataFile::DataFile(const char* filename){
    file = fopen(filename, "r");
    if (!file) throw KBFatal(SDL_GetError());
    sdl_stream = SDL_RWFromFP(file, (SDL_bool)1);
}

DataFile::~DataFile(){
    free(sdl_stream);
}

bool DataFile::checkSignature(){
    Uint32 buffer;
    SDL_RWread(sdl_stream, &buffer, 4, 1);
    return buffer == FILE_SIGNATURE;
}

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

DataFile::FileType DataFile::readFileType(){
    Uint8 buffer;
    SDL_RWread(sdl_stream, &buffer, 1, 1);
    return (FileType)buffer;
}

const char* DataFile::readFileTag(){
    fgets(readBuffer, BUFFER_SIZE, file);
    return readBuffer;
}

bool DataFile::eof(){
    return feof(file);
}