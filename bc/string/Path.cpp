#include "bc/string/Path.hpp"
#include "bc/Debug.hpp"
#include "bc/string/Defines.hpp"
#include "bc/string/Length.hpp"
#include "bc/string/Copy.hpp"
#include "bc/string/Append.hpp"

namespace Blizzard {
namespace String {

static char s_empty[] = { 0 };

char* FindFilename(char* str) {
    char ch      = 0;
    char* result = nullptr;
    auto ptr     = str;

    if (str == nullptr) {
        return s_empty;
    }

    do {
        do {
            result = ptr;
            ch     = *str;
            ptr    = ++str;
        } while (ch == '/');
    } while ((ch == '\\') || (ptr = result, ch != '\0'));
    return result;
}

const char* FindFilename(const char* str) {
    char ch            = 0;
    const char* result = nullptr;
    auto ptr           = str;

    if (str == nullptr) {
        return s_empty;
    }

    do {
        do {
            result = ptr;
            ch     = *str;
            ptr    = ++str;
        } while (ch == '/');
    } while ((ch == '\\') || (ptr = result, ch != '\0'));
    return result;
}

char FirstPathSeparator(const char* str) {
    while (*str) {
        if (*str == '/' || *str == '\\') {
            return *str;
        }

        str++;
    }

    return BC_STRING_PATH_SEPARATOR;
}

void ForceTrailingSeparator(char* buf, int32_t bufMax, char separator) {
    if (buf == nullptr) {
        return;
    }

    if (separator == '\0') {
        separator = FirstPathSeparator(buf);
    }

    // buffer needs at least two characters.
    if (bufMax < 2) {
        return;
    }

    // slice off the end of the path buffer,
    // so that any existing* trailing separator is discarded.
    auto len = Length(buf);
    char last = '\0';
    if (len > 0) {
        last = buf[len - 1];
    }
    switch (last) {
    case '/':
    case '\\':
        len--;
        break;
    default:
        break;
    }

    // Add (back*) trailing separator
    BLIZZARD_ASSERT(len <= bufMax - 2);
    buf[len]     = separator;
    buf[len + 1] = '\0'; // pad null
}

void JoinPath(char* dst, int32_t count, const char* str1, const char* str2) {
    if (dst) {
        Copy(dst, str1, count);
        if (*dst) {
            ForceTrailingSeparator(dst, count, 0);
        }
        if (str2) {
            while (*str2 == '/' || *str2 == '\\') {
                str2++;
            }
        }
        Append(dst, str2, count);
        MakeConsistentPath(dst, dst, count);
    }
}

char* MakeBackslashPath(const char* src, char* buffer, int32_t buffersize) {
    auto dst = buffer;
    if (buffer) {
        auto dstend = buffer + buffersize - 1;
        while (dst < dstend) {
            auto c = *src++;
            if (c == '\0') {
                break;
            } else if (c == '/') {
                *dst = '\\';
            } else {
                *dst = c;
            }
            dst++;
        }
    }

    *dst = '\0';
    return buffer;
}

char* MakeUnivPath(const char* src, char* buffer, int32_t buffersize) {
    auto dst = buffer;
    if (buffer) {
        auto dstend = buffer + buffersize - 1;
        while (dst < dstend) {
            auto c = *src++;
            if (c == '\0') {
                break;
            } else if (c == '\\') {
                *dst = '/';
            } else {
                *dst = c;
            }
            dst++;
        }
    }

    *dst = '\0';
    return buffer;
}

char* MakeConsistentPath(const char* src, char* buffer, int32_t buffersize) {
    if (!buffer || (buffersize <= 0)) {
        return nullptr;
    }

    if (!src || (buffersize == 1)) {
        *buffer = '\0';
        return buffer;
    }

    switch (FirstPathSeparator(src)) {
    case '\\':
        return MakeBackslashPath(src, buffer, buffersize);
    case '/':
        return MakeUnivPath(src, buffer, buffersize);
    }

    Copy(buffer, src, buffersize);
    return buffer;
}

char* MakeNativePath(const char* src, char* buffer, int32_t buffersize) {
    if (!src) {
        *buffer = '\0';
        return buffer;
    } else {
#if defined(WHOA_SYSTEM_WIN)
        return MakeBackslashPath(src, buffer, buffersize);
#else
        return MakeUnivPath(src, buffer, buffersize);
#endif
    }
}

} // namespace String
} // namespace Blizzard
