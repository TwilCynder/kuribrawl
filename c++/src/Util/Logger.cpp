#include "Util/Logger.h"

Logger::Logger(std::ostream& stream_):
    stream(stream_)
{}

inline void Logger::changeColor(const std::string_view& color){
    stream << escapeBegin << changeColorSequence << color << espaceEnd;
}

inline void Logger::changeColor(int color){
    stream << escapeBegin << changeColorSequence << color << espaceEnd;
}

inline void Logger::resetColor(){
    stream << resetColorSequence;
}

Logger::changeColorData Logger::color(int c){
    return (changeColorData)c;
}

Logger& Logger::operator<<(Logger::changeColorData d){
    changeColor(d.v);
    return *this;
}

Logger::resetColorData Logger::reset(){
    return resetColorData();
}

Logger& Logger::operator<<(Logger::resetColorData&&){
    resetColor();
    return *this;
}

Logger& Logger::operator<<(int i){
    changeColor(intColor);
    stream << i;
    resetColor();
    return *this;
}