#ifndef BC_OS_FILE_READ_FILE_HPP
#define BC_OS_FILE_READ_FILE_HPP

#include "bc/os/file/Types.hpp"
#include <cstdint>

int32_t OsReadFile(HOSFILE fileHandle, void* buffer, uint32_t bytesToRead, uint32_t* bytesRead);

#endif