
#include "bc/string/Format.hpp"
#include "bc/string/Translate.hpp"
#include <cstdio>
#include <cstdarg>

namespace Blizzard {
namespace String {

void Format(char* buffer, uint32_t buffersize, const char* format, ...) {
    if (!buffer || buffersize == 0) {
        return;
    }

    if (!format && buffersize >= 1) {
        *buffer = '\0';
        return;
    }

    auto formatNative = format;

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    constexpr size_t translatedSize = 2048;

    // POSIX formatting convention requires %ll for 64-bit printing
    // find & replace all instances of %I64 with %ll
    char translated[translatedSize] = {0};

    Translate(format, translated, translatedSize, "%I64", "%ll");

    formatNative = translated;
#endif
    va_list args;
    va_start(args, format);

    vsnprintf(buffer, buffersize, formatNative, args);

    *buffer = '\0';
}

void VFormat(char* buffer, uint32_t buffersize, const char* format, va_list args) {
    auto formatNative = format;

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    char translatedformat[0x800];
    Translate(format, translatedformat, 0x800, "%I64", "%ll");
    formatNative = buffer;
#endif
    if (format == nullptr) {
        if (buffer) {
            *buffer = '\0';
        }
    } else {
        vsnprintf(buffer, buffersize, formatNative, args);
    }
}


} // namespace String
} // namespace Blizzard
