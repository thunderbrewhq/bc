#include "test/Test.hpp"
#include "bc/Debug.hpp"
#include <cstdint>

#if defined(WHOA_BUILD_ASSERTIONS)

static bool s_assertion_failed = false;

void AssertCallback(const char* a1,  const char* a2, const char* a3, uint32_t a4) {
    s_assertion_failed = true;
}


TEST_CASE("BC_ASSERT", "[debug]") {
    SECTION("assertion fails when evaluating boolean expression") {
        BC_ASSERT(0);
        REQUIRE(s_assertion_failed);
    }
}

#endif
