#pragma once
#include <string>
#include <cstdio>
#include <memory>
#include <ostream>

namespace Kuribrawl {
    //reminder : y a la classe string alternative dans le dosser tests sur le pc noir si j'en ai beosin un jour 

    std::string formatString(const char* format);

    template<typename T, typename... Args>
    std::string formatString(const char* format, T arg1, Args... args){
        int size = std::snprintf(nullptr, 0, format, arg1, args...);
        if( size <= 0 ) throw std::runtime_error( "Error during formatting." );

        std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
        std::snprintf( buf.get(), size + 1, format, arg1, args ... );
        return std::string( buf.get(), buf.get() + size);  
    }
}