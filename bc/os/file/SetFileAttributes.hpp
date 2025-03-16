#ifndef BC_OS_FILE_SET_FILE_ATTRIBUTES_HPP
#define BC_OS_FILE_SET_FILE_ATTRIBUTES_HPP

#include <cstdint>

int32_t OsSetFileAttributes(const char* fileName, uint32_t attributes);

#endif