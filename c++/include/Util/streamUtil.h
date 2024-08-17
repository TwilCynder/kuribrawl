#pragma once

#include <ostream>
#include <functional>

namespace Kuribrawl
{
    void printFixed(std::ostream& l, const char* str, size_t len);
    void printBytes(std::ostream& l, const char* str, size_t len);    
    void printReadable(std::ostream& l, const char);
    void printReadable(std::ostream& l, const char* str, size_t len);
} // namespace Kuribrawl

std::ostream& operator<<(std::ostream&, std::function<void(std::ostream&)>);
