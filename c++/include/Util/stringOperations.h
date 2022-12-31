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
        using iterator = const CharT*;
        using const_iterator = const CharT*;

        basic_string_view():
            str(nullptr), len(0)
        {}

        basic_string_view(const CharT* data){
            set(data);
        }

        basic_string_view(const CharT* data, size_t l){
            set(data, l);
        }

        basic_string_view(const basic_string_view& sv):
            str(sv.str), len(sv.len)
        {}

        basic_string_view& operator=(const CharT* data){
            set(data);
            return *this;
        }

        basic_string_view& operator=(const basic_string_view& sv){
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

        const CharT* data() const {
            return str;
        }

        const CharT* cdata() const {
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

        void set(const CharT* data, const size_t l){
            str = data;
            len = l;
        }

        void set(const CharT* data){
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

        void copy(const CharT* dest) const {
            memcpy(dest, str, len);
        }

        /**
         * @brief Returns a pointer to the first occurence of c in the string, or null pointer if there is nonte
         * 
         * @param c character to find
         * @return CharT* 
         */
        const CharT* find(const CharT& c) const {
            return Traits::find(str, len, c);
            /*for (CharT* ptr = str; ptr < (str + len); ptr++)
                if (*ptr == c)
                    return ptr;

            return nullptr;
            */
        }

        int pos(const CharT& c) const {
            for (size_t i = 0; i < len; i++)
                if (str[i] == c)
                    return i;
                    
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

        int8_t compare(const CharT* const ostr, size_t olen) const{
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

        operator std::basic_string<CharT>() const{
            return std::basic_string<CharT>(str, len);
        }

        operator std::basic_string_view<CharT>() const{
            return std::basic_string_view<CharT>(str, len);
        }

        template<typename C>
        friend std::ostream& operator<<(std::basic_ostream<C>& os, basic_string_view<C>& sv);

    };
    using string_view = basic_string_view<char>;
    
    template <typename C>
    using const_basic_string_view = basic_string_view<const C>;
    using string_viewC = const_basic_string_view<char>;

    template<typename C>
    std::ostream& operator<<(std::basic_ostream<C>& os, basic_string_view<C>& sv){
        // Function from : 
        // https://codereview.stackexchange.com/q/242428?stw=2 
        std::ostream::sentry sentry{os};
        if (!sentry) return os;

        // Ideas from:
        // https://stackoverflow.com/q/39653508

        /*size_t padding = 0;
        char filler = os.fill();
        if (s.size() < os.width()) {
            padding = os.width() - s.size();
        }

        bool align_left = os.flags() & std::ios_base::left;
        if (padding > 0 && !align_left) {
            while (padding--) os.put(filler);
        }*/
        os.write(sv.str, sv.len);
        /*if (padding > 0 && align_left) {
            while (padding--) os.put(filler);
        }

        os.width(0);*/
        return os;
    }

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

    const char* getEscape(const char);
}