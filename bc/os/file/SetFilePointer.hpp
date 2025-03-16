#ifndef BC_OS_FILE_SET_FILE_POINTER_HPP
#define BC_OS_FILE_SET_FILE_POINTER_HPP

#include <cstdint>
#include "bc/os/file/Types.hpp"

int64_t OsSetFilePointer(HOSFILE fileHandle, int64_t distanceToMove, uint32_t moveMethod);

#endif