#ifndef BC_DEBUG_HPP
#define BC_DEBUG_HPP

#include "bc/system/System_Debug.hpp"
#include <cstdint>

#if defined(WHOA_ASSERTIONS_ENABLED)
#define BC_ASSERT(x)                               \
    if (!(x)) {                                          \
        Blizzard::Debug::Assert(#x, __FILE__, __LINE__); \
    }                                                    \
    (void)0
#else
#define BC_ASSERT(x) \
    (void)0
#endif

#define BC_VALIDATE(x, y, ...)                      \
    if (!(x)) {                                           \
        Blizzard::Debug::Assert(#y, __FILE__, __LINE__); \
        return __VA_ARGS__;                               \
    }                                                     \
    (void)0

namespace Blizzard {
namespace Debug {

// Types

class ErrorStackRecord {
    public:
};

// Functions
void Assert(const char* a1, const char* a2, uint32_t a3);
void SetAssertHandler(Blizzard::System_Debug::AssertCallback callback);

} // namespace Debug
} // namespace Blizzard

#endif
