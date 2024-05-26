#pragma once
#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <functional>
#include <bitset>
#include "type_wrapper.h"
#include "LMFWrapper.h"

class Logger {
    std::ostream* stream;

    struct changeColorData_d {};
    using changeColorData = type_wrapper<int, changeColorData_d>;
    struct resetColorData {};

    public:
    Logger(std::ostream&);

    void newline();

    static constexpr std::string_view escapeBegin = "\033[";
    static constexpr char espaceEnd = 'm';
    static constexpr std::string_view changeColorSequence = "38;5;";
    static constexpr std::string_view resetColorSequence = "\033[0m";

    inline void changeColor(const std::string_view& color);
    inline void changeColor(int color);
    inline void resetColor();

    static changeColorData color(int c);
    Logger& operator<<(changeColorData d);

    static resetColorData reset();
    Logger& operator<<(resetColorData&&);

    static constexpr std::string_view intColor = "220";
    Logger& operator<<(int i);
    Logger& operator<<(long i);

    using ostreamFunction = std::ostream& (*)(std::ostream&);
    Logger& operator<<(ostreamFunction);

    using loggerFunction = void (*)(Logger &);
    Logger& operator<<(loggerFunction);

    using lambda = std::function<void(Logger&)>;
    Logger& operator<<(lambda&&);

    
    template <class T, std::ostream&(T::*F)(std::ostream&)>
    Logger& operator<<(const LogWrapper<T, F> lw){
        stream = &lw(*stream);
        return *this;
    }

        template <class T, std::ostream&(T::*F)(std::ostream&) const>
    Logger& operator<<(const ConstLogWrapper<T, F> lw){
        stream = &lw(*stream);
        return *this;
    }

    template<typename T>
    static lambda hex(const T& val){
        return [val](Logger& l){
            l << std::hex;
            l << val;
            l << std::dec;
        };
    }

    template <typename T>
    static lambda bin(const T& val){
        return [val](Logger& l){
            l << std::bitset<sizeof(T) * 8>(val);
        };
    }

    template<typename T>
    Logger& operator<<(T&& v){
        (*stream) << v;
        return *this;
    }

    template<typename T>
    Logger& operator<<(const T& v){
        (*stream) << v;
        return *this;
    }

    constexpr static auto col = color;
    constexpr static auto rst = reset;

    void printFixed(const char*, size_t);
    void printBytes(const char*, size_t);
    void printReadable(const char);
    void printReadable(const char*, size_t len);

};

class SpacedLogger {
    Logger& log;

    public:

    SpacedLogger(Logger& log_) : 
        log(log_)
    {}

    template<typename T>
    SpacedLogger& operator<<(T arg){
        log << arg << ' ';
        return *this;
    }
    
    SpacedLogger& operator<<(char);
};

using Log = Logger;
using SLog = SpacedLogger;