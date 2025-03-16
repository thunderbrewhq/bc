#include "bc/string/Length.hpp"
#include <cstring>

uint32_t Blizzard::String::Length(const char* str) {
    if (str) {
        return strlen(str);
    }

    return 0;
}
