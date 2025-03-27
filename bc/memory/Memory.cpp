#include "bc/memory/Memory.hpp"
#include "bc/memory/Storm.hpp"

namespace Blizzard {
namespace Memory {

void* Allocate(uint32_t bytes) {
    return SMemAlloc(bytes, __FILE__, __LINE__, 0x0);
}

void* Allocate(uint32_t bytes, uint32_t flags, const char* filename, uint32_t linenumber, const char* a5) {
    // TODO
    // - determine purpose of a5
    // - flags manipulation

    return SMemAlloc(bytes, filename, linenumber, flags);
}

void Free(void* ptr) {
    SMemFree(ptr);
}

} // namespace Memory
} // namespace Blizzard
