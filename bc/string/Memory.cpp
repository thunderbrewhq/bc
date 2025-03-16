#include "bc/string/Memory.hpp"

#include <cstdint>
#include <cstring>

namespace Blizzard {
namespace String {

void MemFill(void* dst, int32_t count, uint8_t fill) {
    memset(dst, fill, count);
}

int32_t MemCompare(void* p1, void *p2, int32_t count) {
    return memcmp(p1, p2, count);
}

void MemCopy(void* dst, const void* src, int32_t count) {
    memmove(dst, src, count);
}

} // namespace String
} // namespace Blizzard
