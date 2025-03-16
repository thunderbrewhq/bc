#ifndef BC_OS_FILE_COPY_FILE_HPP
#define BC_OS_FILE_COPY_FILE_HPP

#include <cstdint>

int32_t OsCopyFile(const char* existingFileName, const char* newFileName, int32_t failIfExists);

#endif