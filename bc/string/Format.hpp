#ifndef BC_STRING_FORMAT_HPP
#define BC_STRING_FORMAT_HPP

#include <cstdint>
#include <cstdarg>

namespace Blizzard {
namespace String {

void Format(char* buffer, uint32_t buffersize, const char* format, ...);

void VFormat(char* buffer, uint32_t buffersize, const char* format, va_list args);

} // namespace String
} // namespace Blizzard

#endif
