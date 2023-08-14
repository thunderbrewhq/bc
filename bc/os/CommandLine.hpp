#ifndef BC_OS_COMMAND_LINE_HPP
#define BC_OS_COMMAND_LINE_HPP

#include <cstdint>

const char* OsGetCommandLine();

void OsSetCommandLine(int32_t argc, char** argv);

#endif
