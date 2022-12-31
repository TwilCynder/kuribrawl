#include "Util/Logger.h"
#include "Util/streamUtil.h"

Logger::Logger(std::ostream& stream_):
    stream(stream_)
{}

void Logger::newline(){
    stream << '\n';
}

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

Logger& Logger::operator<<(long i){
    changeColor(intColor);
    stream << i;
    resetColor();
    return *this;
}

Logger& Logger::operator<<(Logger::ostreamFunction f){
    stream << f;
    return *this;
}

Logger& Logger::operator<<(loggerFunction f){
    f(*this);
    return *this;
}

Logger &Logger::operator<<(lambda&& l)
{
    l(*this);
    return *this;
}

void Logger::printFixed(const char* str, size_t len){
    Kuribrawl::printFixed(stream, str, len);
}

void Logger::printBytes(const char* str, size_t len){
    Kuribrawl::printBytes(stream, str, len);
}

void Logger::printReadable(const char c){
    Kuribrawl::printReadable(stream, c);
}

void Logger::printReadable(const char* str, size_t len){
    Kuribrawl::printReadable(stream, str, len);
}

SpacedLogger& SpacedLogger::operator<<(char c)
{
    if (c == '\n'){
        log << c;
    } else {
        log << c << ' ';
    }
    return *this;
}
