#include "bc/string/Append.hpp"
#include <cstring>

namespace Blizzard {
namespace String {

int32_t Append(char* dst, const char* src, uint32_t count) {
    if (count == 0 || dst == nullptr) {
        return 0;
    }

    auto dstend = dst + count;
    while (dst < dstend && *dst) {
        dst++;
    }
    if (dstend <= dst) {
        return 0;
    }

    auto dststart = dst;

    while (dst < dstend && *src) {
        *dst++ = *src++;
    }
    *dst = '\0';

    return dst - dststart;
}

} // namespace String
} // namespace Blizzard
