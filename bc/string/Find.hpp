#ifndef BC_STRING_FIND_HPP
#define BC_STRING_FIND_HPP

#include <cstdint>

namespace Blizzard {
namespace String {

char* Find(char* str, char ch, int32_t count);

const char* Find(const char* str, char ch, int32_t count);

} // namespace String
} // namespace Blizzard

#endif
