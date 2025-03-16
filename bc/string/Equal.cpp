#include "bc/string/Equal.hpp"
#include <cstring>

namespace Blizzard {
namespace String {

bool Equal(const char* a, const char* b) {
    if (a && b) {
        return 0 == strcmp(a, b);
    }
    return false;
}

bool EqualI(const char* a, const char* b, uint32_t count) {
    if (a && b) {
#if defined(WHOA_SYSTEM_WIN)
        return 0 == _strnicmp(a, b, count);
#else
        return 0 == strncasecmp(a, b, count);
#endif
    }
    return false;
}

} // namespace String
} // namespace Blizzard

