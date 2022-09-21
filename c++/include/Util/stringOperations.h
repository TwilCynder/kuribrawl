#pragma once
#include <string>
#include <cstdio>
#include <memory>
#include <ostream>

namespace Kuribrawl {
    //reminder : y a la classe string alternative dans le dosser tests sur le pc noir si j'en ai beosin un jour 

    template<typename... Args>
    std::string formatString(const std::string_view& format, Args... args){
        int size = std::snprintf(nullptr, 0, format.data(), args...);
        if( size <= 0 ) throw std::runtime_error( "Error during formatting." );

        std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
        std::snprintf( buf.get(), size + 1, format.data(), args ... );
        return std::string( buf.get(), buf.get() + size);  
    }
}