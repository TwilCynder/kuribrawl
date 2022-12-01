#pragma once
#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include "type_wrapper.h"

class Logger {
    std::ostream& stream;

    struct changeColorData_d {};
    using changeColorData = type_wrapper<int, changeColorData_d>;
    struct resetColorData {};

    public:
    Logger(std::ostream&);

    template<typename T>
    Logger& operator<<(T&& v){
        stream << v;
        return *this;
    }

    template<typename T>
    Logger& operator<<(const T& v){
        stream << v;
        return *this;
    }

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

    constexpr static auto col = color;
    constexpr static auto rst = reset;
};

using Log = Logger;
