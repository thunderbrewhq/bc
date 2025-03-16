#ifndef BC_STRING_COPY_HPP
#define BC_STRING_COPY_HPP

#include <cstdint>

namespace Blizzard {
namespace String {

int32_t Copy(char* dst, const char* src, uint32_t count);

} // namespace String
} // namespace Blizzard

#endif
