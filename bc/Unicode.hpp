#ifndef BC_UNICODE_HPP
#define BC_UNICODE_HPP

#include <cstdint>

namespace Blizzard {
namespace Unicode {

int32_t ConvertUTF16to8(uint8_t* dst, uint32_t dstmaxchars, const uint16_t* src, uint32_t srcmaxchars, uint32_t* dstchars, uint32_t* srchars);

int32_t ConvertUTF8to16(uint16_t* dst, uint32_t dstmaxchars, const uint8_t* src, uint32_t srcmaxchars, uint32_t* dstchars, uint32_t* srcchars);

int32_t ConvertUTF8to16Len(const uint8_t* src, uint32_t srcmaxchars, uint32_t* srcchars);

int32_t GetCodepointFromUTF8(const uint8_t** c);

} // namespace Unicode
} // namespace Blizzard

#endif
