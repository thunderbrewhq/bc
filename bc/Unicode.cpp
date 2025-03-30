#include "bc/Unicode.hpp"
#include "bc/Debug.hpp"
#include <cstdio>
#include <limits>

namespace Blizzard {
namespace Unicode {

static const uint32_t offsetsFromUTF8[] = {
    0x0,
    0x0,
    0x3080,
    0xE2080,
    0x3C82080,
    0xFA082080,
    0x82082080
};

static const uint32_t firstByteMark[] = {
    0x0,
    0x0,
    0xC0,
    0xE0,
    0xF0,
    0xF8,
    0xFC
};

static const uint8_t bytesFromUTF8[] = {
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06
};

int32_t ConvertUTF16to8(uint8_t* dst, uint32_t dstmaxchars, const uint16_t* src, uint32_t srcmaxchars, uint32_t* dstchars, uint32_t* srcchars) {
    auto srcend   = srcmaxchars & 0x80000000 ? reinterpret_cast<const uint16_t*>(UINTPTR_MAX) : &src[srcmaxchars];
    auto dstend   = &dst[dstmaxchars];
    auto dststart = dst;
    auto srcstart = src;

    int32_t result;

    while (src < srcend) {
        uint32_t widechars = 1;
        auto grapheme      = static_cast<uint32_t>(src[0]);
        if (0xD7FF < grapheme && grapheme < 0xDC00) {
            if (src + 1 >= srcend) {
                goto fail;
            }
            auto char2 = static_cast<uint32_t>(src[1]);
            if (0xDBFF < char2 && char2 < 0xE000) {
                grapheme  = ((grapheme - 0xD7F7) * 1024) + char2;
                widechars = 2;
            }
        }

        uint32_t chars;

        if (grapheme < 0x80) {
            chars = 1;
            if (grapheme == 0) {
                if (dst < dstend) {
                    *dst++ = '\0';
                    result = 0;
                }
                goto done;
            }
        } else if (grapheme < 0x800) {
            chars = 2;
        } else if (grapheme < 0x10000) {
            chars = 3;
        } else if (grapheme < 0x200000) {
            chars = 4;
        } else if (grapheme < 0x4000000) {
            chars = 5;
        } else if (grapheme > 0x7FFFFFFF) {
            grapheme = 0xFFFD;
            chars    = 2;
        } else {
            chars = 6;
        }

        result = chars;

        dst += chars;

        if (dst > dstend) {
            goto done;
        }

        switch (chars) {
        case 6:
            *--dst = (grapheme & 0x3F) | 0x80;
            grapheme >>= 6;
        case 5:
            *--dst = (grapheme & 0x3F) | 0x80;
            grapheme >>= 6;
        case 4:
            *--dst = (grapheme & 0x3F) | 0x80;
            grapheme >>= 6;
        case 3:
            *--dst = (grapheme & 0x3F) | 0x80;
            grapheme >>= 6;
        case 2:
            *--dst = (grapheme & 0x3F) | 0x80;
            grapheme >>= 6;
        case 1:
            *--dst = grapheme | firstByteMark[chars];
        }

        src += widechars;
        dst += chars;
    }

fail:
    result = -1;

done:
    if (srcchars) {
        *srcchars = src - srcstart;
    }

    if (dstchars) {
        *dstchars = dst - dststart;
    }
    return result;
}

int32_t ConvertUTF8to16(uint16_t* dst, uint32_t dstmaxchars, const uint8_t* src, uint32_t srcmaxchars, uint32_t* dstchars, uint32_t* srcchars) {
    auto srcend   = srcmaxchars & 0x80000000 ? reinterpret_cast<const uint8_t*>(UINTPTR_MAX) : src + srcmaxchars;
    auto dstend   = dst + dstmaxchars;
    auto dststart = dst;
    auto srcstart = src;

    int32_t result = 0;

    while (src < srcend) {
        auto bytes = bytesFromUTF8[*src];
        if ((src + bytes) > srcend) {
            result = -bytes;
            goto done;
        }

        uint32_t grapheme = 0;

        switch (bytes) {
        case 6:
            grapheme = *src++ << 6;
        case 5:
            grapheme = (grapheme + *src++) << 6;
        case 4:
            grapheme = (grapheme + *src++) << 6;
        case 3:
            grapheme = (grapheme + *src++) << 6;
        case 2:
            grapheme = (grapheme + *src++) << 6;
        case 1:
            grapheme = (grapheme + *src++) - offsetsFromUTF8[bytes];
        }

        if (dst >= dstend) {
            result = 1;
            goto done;
        }

        if (grapheme < 0x10000) {
            *dst++ = static_cast<uint16_t>(grapheme);
            if (grapheme == 0x0) {
                goto done;
            }
        } else if (grapheme < 0x110000) {
            if (dst >= dstend) {
                result = 1;
                goto done;
            }
            auto v16 = grapheme - 0x10000;
            *dst++   = static_cast<uint16_t>(v16 >> 10)   - 0x2800;
            *dst++   = static_cast<uint16_t>(v16 & 0x3FF) - 0x2400;
        } else {
            *dst++ = 0xFFFD;
        }
    }

    result = -1;

done:
    if (srcchars) {
        *srcchars = src - srcstart;
    }

    if (dstchars) {
        *dstchars = dst - dststart;
    }
    return result;
}

int32_t ConvertUTF8to16Len(const uint8_t* src, uint32_t srcmaxchars, uint32_t* srcchars) {
    // TODO
    BC_ASSERT(0);
    return -1;
}

int32_t GetCodepointFromUTF8(const uint8_t** c) {
    // TODO
    BC_ASSERT(0);
    return -1;
}

} // namespace Unicode
} // namespace Blizzard
