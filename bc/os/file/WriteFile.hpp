#ifndef BC_OS_FILE_WRITE_FILE_HPP
#define BC_OS_FILE_WRITE_FILE_HPP

#include "bc/os/file/Types.hpp"
#include <cstdint>

int32_t OsWriteFile(HOSFILE fileHandle, void* buffer, uint32_t bytesToWrite, uint32_t* bytesWritten);

#endif