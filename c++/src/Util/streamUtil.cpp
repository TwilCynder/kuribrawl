#include <cctype>
#include "Util/streamUtil.h"
#include "Util/stringOperations.h"

void Kuribrawl::printFixed(std::ostream& l, const char* str, size_t len){
    const char * const bound = str + len;
    for (; str < bound; str++){
        l << *str;
    }
}

void Kuribrawl::printBytes(std::ostream & l, const char *str, size_t len){
    const char * const bound = str + len;
    for (; str < bound; str++){
        l << (int)*str << ' ';
    }
}

void Kuribrawl::printReadable(std::ostream &l, const char c){
    if (std::isprint(c)){
        l << c;
        return;
    }

    const char* esc = getEscape(c);
    if (esc){
        l << esc;
    } else {
        l << "\\u" << +c;
    }
}

void Kuribrawl::printReadable(std::ostream& l, const char* str, size_t len){
    const char * const bound = str + len;
    for (; str < bound; str++){
        printReadable(l, *str);
    }
}

std::ostream &operator<<(std::ostream & os, std::function<void(std::ostream &)> f)
{   
    f(os);
    return os;
}