#ifndef BC_STRING_QUICK_FORMAT_HPP
#define BC_STRING_QUICK_FORMAT_HPP

#include "bc/string/Format.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdarg>

namespace Blizzard {
namespace String {

template<uint32_t N>
class QuickFormat {
    private:
        char buffer[N];
    public:
        QuickFormat(const char* format, ...) {
            va_list args;
            va_start(args, format);
            VFormat(this->buffer, N, format, args);
        }

        const char* ToString() {
            return this->buffer;
        }
};

} // namespace String
} // namespace Blizzard

#endif
