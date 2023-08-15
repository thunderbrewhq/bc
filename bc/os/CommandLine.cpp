#include "bc/os/CommandLine.hpp"
#include "bc/Debug.hpp"

#include <string>
#include <cstring>

#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

// Variables

char commandline[1024] = {0};

// Functions

const char* OsGetCommandLine() {
#if defined(WHOA_SYSTEM_WIN)
    return GetCommandLine();
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    return commandline;
#endif
}

std::string QuoteArgument(std::string argument) {
    std::string result = "";

    result += "\"";
    result += argument;
    result += "\"";

    return result;
}

std::string CheckArgument(std::string argument) {
    for (size_t i = 0; i < argument.length(); i++) {
        switch (argument.at(i)) {
        case '\"':
        case ' ':
            return QuoteArgument(argument);
        }
    }

    return argument;
}

void OsSetCommandLine(int32_t argc, char** argv) {
    int32_t i;
    std::string result = "";

    while (i < argc) {
        if (i > 0) {
            result += " ";
        }

        result += CheckArgument(argv[i]);
        i++;
    }

    strncpy(commandline, result.c_str(), sizeof(commandline));
}
