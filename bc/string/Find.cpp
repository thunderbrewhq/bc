#include "bc/string/Find.hpp"

char* Blizzard::String::Find(char* str, char ch, int32_t count) {
    if (!str || !ch) {
        return nullptr;
    }

    auto ptr = str;
    auto end = str + count;

    while (ptr != end && *ptr) {
        if (ch == *ptr) {
            return ptr;
        }

        ptr++;
    }

    return nullptr;
}

const char* Find(const char* str, char ch, int32_t count) {
    if (!str || !ch) {
        return nullptr;
    }

    auto ptr = str;
    auto end = str + count;

    while (ptr != end && *ptr) {
        if (ch == *ptr) {
            return ptr;
        }

        ptr++;
    }

    return nullptr;
}
