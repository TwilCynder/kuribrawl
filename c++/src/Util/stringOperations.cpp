#include "Util/stringOperations.h"
#include <ostream>
#include <map>

std::string Kuribrawl::formatString(const char* format){
    return std::string(format);
}

std::map<char, const char*> escapes = {
    {'\a', "\\a"},
    {'\b', "\\b"},
    {'\f', "\\f"},
    {'\n', "\\n"},
    {'\r', "\\r"},
    {'\t', "\\t"},
    {'\v', "\\v"},
    {'\\', "\\\\"},
    {'\'', "\\\'"},
    {'\"', "\\\""},
    {'\?', "\\\?"},
    {0, "\\0"}
};

const char* Kuribrawl::getEscape(const char c){
    auto res = escapes.find(c);
    if (res == escapes.end())
        return nullptr;
    return res->second;
}
