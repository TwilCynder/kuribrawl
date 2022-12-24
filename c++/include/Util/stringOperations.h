#pragma once
#include <string>
#include <string_view>
#include <cstdio>
#include <memory>
#include <ostream>

namespace Kuribrawl {

    //forget the std::string_view bullshit, al you need is a (mutable) pointer with a (mutable) length
    template <typename CharT, class Traits = std::char_traits<CharT>>
    class basic_string_view {
        const CharT* str;
        size_t len;

        public:
        using iterator = CharT*;
        using const_iterator = const CharT*;

        basic_string_view():
            str(nullptr), len(0)
        {}

        basic_string_view(CharT* data){
            set(data);
        }

        basic_string_view(CharT* data, size_t l){
            set(data, l);
        }

        basic_string_view(const basic_string_view& sv):
            str(sv.str), len(sv.len)
        {}

        operator=(CharT* data){
            set(data);
            return *this;
        }

        operator=(const basic_string_view& sv){
            set(sv.str, sv.len);
            return *this;
        }

        iterator begin(){
            return str; 
        }

        iterator end(){
            return str + len;
        }

        const_iterator cbegin() const {
            return str;
        }

        const_iterator cend() const {
            return str + len;
        }

        CharT* data(){
            return str;
        }

        size_t size() const {
            return len;
        }

        size_t length() const {
            return size();
        }

        CharT at(size_t i){
            if (i >= len){
                throw std::out_of_range();
            }
            return str[i];
        }

        CharT operator[](size_t i){
            return str[i];
        }

        void set(CharT* data, size_t l){
            str = data;
            len = l;
        }

        void set(CharT* data){
            str = data;
            len = Traits::length(data);
        }

        void set(size_t l){
            len = l;
        }

        /**
         * @brief Creates a new basic_string_view on a new array of chars ; must be deleted !
         * 
         * @param len 
         * @return basic_string_view 
         */
        static basic_string_view make(size_t len){
            return basic_string_view(new CharT[len], len);
        }

        void copy(CharT* dest) const {
            memcpy(dest, str, len);
        }

        /**
         * @brief Returns a pointer to the first occurence of c in the string, or null pointer if there is nonte
         * 
         * @param c character to find
         * @return CharT* 
         */
        CharT* find(const CharT& c) {
            return Traits::find(str, len, c);
        }

        int pos(const CharT& c) const {
            for (size_t i = 0; i < len; i++){
                if (str[i] == c){
                    return i;
                }
            }
            return -1;
        }

        int find_any_of(const basic_string_view& sv){
            for (size_t i = 0; i < len; i++)
                for (size_t j = 0; j < sv.len; j++)
                    if (str[i] == sv[j])
                        return i;

            return -1;
        }

        bool empty() const {
            return len < 1;
        }

        int8_t compare(const CharT* const ostr, size_t olen){
            size_t rlen = std::min(len, olen);
            int8_t comparison = Traits::compare(str, ostr, rlen);
            if (comparison == 0)
                return len - olen;
            return comparison;
        }

        int8_t operator<=>(const basic_string_view& sv) const {
            return compare(sv.str, sv.len);
        }

        int8_t operator<=>(const std::basic_string_view<CharT, Traits>& sv) const {
            return compare(sv.data(), sv.size());
        }

        int8_t operator<=>(const std::basic_string<CharT, Traits>& s) const {
            return compare(s.c_str(), s.size());
        }

        operator std::basic_string<CharT>(){
            return std::basic_string(str, len);
        }

        operator std::basic_string_view<CharT>(){
            return std::basic_string_view<CharT>(str, len);
        }

    };
    using string_view = basic_string_view<char>;


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