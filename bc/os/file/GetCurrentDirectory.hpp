#ifndef BC_OS_FILE_GET_CURRENT_DIRECTORY_HPP
#define BC_OS_FILE_GET_CURRENT_DIRECTORY_HPP

#include <cstdint>

int32_t OsGetCurrentDirectory(uint32_t buffersize, char* buffer);

#endif