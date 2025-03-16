#ifndef BC_STRING_EQUAL_HPP
#define BC_STRING_EQUAL_HPP

#include <cstdint>

namespace Blizzard {
namespace String {

bool Equal(const char* a, const char* b);

bool EqualI(const char* a, const char* b, uint32_t count);

} // namespace String
} // namespace Blizzard

#endif
