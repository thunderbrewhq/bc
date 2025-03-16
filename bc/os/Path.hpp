#ifndef BC_OS_PATH_HPP
#define BC_OS_PATH_HPP

#include <cstdint>

void OsGetExePath(char* buffer, uint32_t buffersize);

void OsGetExeName(char* buffer, uint32_t buffersize);

#endif
