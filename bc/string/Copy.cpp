#include "bc/string/Copy.hpp"
#include <cstring>

int32_t Blizzard::String::Copy(char* dst, const char* src, uint32_t count) {
    if (!count || !dst) {
        return 0;
    }

    if (!src) {
        *dst = 0;
        return 0;
    }

    char* v3 = dst + count - 1;

    char v4;
    const char* v5;
    char* v6;

    int32_t result;

    if (dst < v3 && (v4 = *src, v5 = src, v6 = dst, *src)) {
        do {
            *v6++ = v4;

            if (v3 <= v6) {
                break;
            }

            v4 = (v5++)[1];
        } while (v4);

        result = v6 - dst;
    } else {
        v6 = dst;
        result = 0;
    }

    *v6 = 0;

    return result;
}
