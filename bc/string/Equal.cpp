#include "bc/string/Equal.hpp"
#include <cstring>

namespace Blizzard {
namespace String {

bool Equal(const char* a, const char* b) {
    if (a && b) {
        return strcmp(a, b);
    }
    return false;
}

bool EqualI(const char* a, const char* b, uint32_t count) {
    if (a && b) {
        return strncasecmp(a, b, count);
    }
    return false;
}

} // namespace String
} // namespace Blizzard

