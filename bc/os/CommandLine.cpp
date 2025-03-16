#include "bc/os/CommandLine.hpp"
#include "bc/string/Append.hpp"

#include <cstring>

#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

// Variables

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
char commandline[65535];
#endif

// Functions

// CUSTOM: appends a command-line argument to the buffer,
void append_commandline(char* buffer, int32_t buffersize, const char* argument) {
    bool escape = false;
    if (*buffer) {
        Blizzard::String::Append(buffer, " ", buffersize);

        auto a = argument;
        while (*a++) {
            if (*a == ' ' || *a == '"') {
                escape = true;
                break;
            }
        }
    } else {
        // first argument is always quoted
        escape = true;
    }

    if (escape) {
        auto a = argument;

        auto dst = buffer;
        auto dstend = buffer + buffersize - 1;
        while (dst < dstend && *dst) {
            dst++;
        }

        if (dst < dstend) {
            *dst++ = '"';
        }

        while (dst < dstend && *a) {
            if (*a == '"') {
                if (dst+1 == dstend) {
                    break;
                }
                *dst++ = '\\';
                *dst++ = '"';
            } else {
                *dst++ = *a;
            }
            a++;
        }

        if (dst < dstend) {
            *dst++ = '"';
        }

        *dst = '\0';
    } else {
        Blizzard::String::Append(buffer, argument, buffersize);
    }
}

const char* OsGetCommandLine() {
#if defined(WHOA_SYSTEM_WIN)
    return GetCommandLine();
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    return commandline;
#endif
}

void OsSetCommandLine(int argc, char* argv[]) {
#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    int i = 0;
    while (i < argc) {
        append_commandline(commandline, sizeof(commandline), argv[i]);
        i++;
    }
#endif
}
