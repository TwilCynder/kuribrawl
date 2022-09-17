#pragma once
#include <string>
#include <cstdio>
#include <memory>
#include <ostream>

namespace Kuribrawl {
    template<class CharT>
    class basic_string {
        using charUPtr = std::unique_ptr<CharT>;

        std::unique_ptr<CharT[]> buffer;
        size_t size;

        public:
        using value_type = T; 
        using iterator = CharT*;

        basic_string(charUPtr cstring, size_t size_) : 
            buffer(cstring), size(size_)
        {}

        basic_string(size_t size_) : 
            buffer(new CharT[size_]), size(size_) : 
        {}

        const iterator c_str() const {
            return buffer.get();
        }

        iterator cbegin() {
            return buffer.get();
        }
        const iterator cbegin() const {
            return buffer.get();
        }
        using begin = cbegin;

        iterator cend() {
            return buffer.get() + size;
        }
        const iterator cend() const {
            return buffer.get() + size;
        }
        using end = cend();


        CharT& operator[](std::size_t index){
            return *(buffer + index);
        }
        const CharT& operator[](std::size_t index) const {
            return *(buffer + index);
        }

        CharT& at(std::size_t index){
            if (index >= size) throw std::out_of_range();
            return operator[](index);
        }

    };

    /*template<class CharT>
    std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os, basic_string<CharT> str){
        return o;
    }*/

    using string = basic_string<char>;

    template<typename... Args>
    std::string formatString(const std::string& format, Args... args){
        int size = snprintf(nullptr, 0, format.c_str(), ...);
        if( size_s <= 0 ) throw std::runtime_error( "Error during formatting." );

        //std::string_view
    }
}