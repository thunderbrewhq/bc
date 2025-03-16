#ifndef BC_STRING_MEMORY_HPP
#define BC_STRING_MEMORY_HPP

#include <cstdint>

namespace Blizzard {
namespace String {

void MemFill(void* dst, int32_t count, uint8_t fill);

int32_t MemCompare(void* p1, void *p2, int32_t count);

void MemCopy(void* dst, const void* src, int32_t count);

} // namespace String
} // namespace Blizzard

#endif
