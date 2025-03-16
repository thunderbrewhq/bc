#include "bc/string/Translate.hpp"
#include "bc/string/Length.hpp"
#include "bc/string/Memory.hpp"
#include <cstring>

void Blizzard::String::Translate(const char* src, char* dst, int32_t dstmaxchars, const char* find, const char* replace) {
    if (dst == nullptr || find == nullptr || replace == nullptr) {
        return;
    }

    if (src == nullptr) {
        src = dst;
    }

    auto srcstart = src;
    auto dststart = dst;

    auto dstmax = &dst[dstmaxchars - 1];
    auto srcmax = &src[Length(src) + 1];

    auto findlength    = Length(find);
    auto replacelength = Length(replace);
    while (dst < dstmax && src < srcmax) {
        auto srcchar = *src;

        if (srcchar == '\0') {
            break;
        }

        if (srcchar == find[0]) {
            auto copylength = replacelength;
            if ((dst + copylength) >= dstmax) {
                copylength = dstmax - dst;
            }

            auto substring = strstr(src, find);
            if (substring == src) {
                MemCopy(dst, replace, copylength);
                dst += replacelength;
                src += findlength;
                continue;
            }
        }

        *dst++ = *src++;
    }

    *dst = '\0';
}
