#ifndef BC_STRING_TRANSLATE_HPP
#define BC_STRING_TRANSLATE_HPP

#include <cstdint>

namespace Blizzard {
namespace String {

void Translate(const char* src, char* dst, int32_t dstmaxchars, const char* find, const char* replace);

} // namespace String
} // namespace Blizzard

#endif
