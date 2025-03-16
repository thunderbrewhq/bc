#include "bc/file/Error.hpp"

#include <cstdio>
#include <cinttypes>

namespace Blizzard {
namespace File {

// Functions

void SetLastError(int32_t errorcode) {
    // TODO
}

void AddToLastErrorStack(int32_t errorcode, const char* msg, int32_t a3) {
    // TODO: use proper logging

    char empty[] = "";

    if (!msg) {
        msg = empty;
    }

    fprintf(stderr, "[bc/file] %" PRIi32 ": '%s'\n", errorcode, msg);
}

}
}
