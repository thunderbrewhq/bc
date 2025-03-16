#include "bc/file/SimpleGlob.hpp"

#include <cstdint>
#include <cctype>

namespace Blizzard {
namespace File {

bool SimpleGlob(const char* name, const char* pattern) {
    auto p = pattern;
    auto n = name;

    while (true) {
        if (*p == '\0') {
            return !*n;
        }
        if (!*n && *p != '*') {
            return false;
        }
        if (*p == '*') {
            break;
        }
        if (*p == '?') {
            if (!*n) {
                return false;
            }
        } else {
            if (*p == '\\' && p[1]) {
                p++;
            }
            if (*p != *n) {
                if (tolower(static_cast<uint8_t>(*n)) != tolower(static_cast<uint8_t>(*p))) {
                    return false;
                }
            }
        }

        n++;
        p++;
    }

    while (*p == '*') {
        p++;
    }

    if (*p) {
        if (*p != '?' && *p != '\\') {
            if (!*n) {
                return false;
            }
            while (*p != *n) {
                if (!*++n) {
                    return false;
                }
            }
        }
        if (!*n) {
            return false;
        }

        while (!SimpleGlob(n, p)) {
            n++;
            if (*n == '\0') {
                return false;
            }
        }
    }
    return true;
}

} // namespace File
} // namespace Blizzard