#ifndef BC_OS_FILE_CREATE_FILE_HPP
#define BC_OS_FILE_CREATE_FILE_HPP

#include <cstdint>

#include "bc/os/file/Types.hpp"

HOSFILE OsCreateFile(const char* fileName, uint32_t desiredAccess, uint32_t shareMode, uint32_t createDisposition, uint32_t flagsAndAttributes, uint32_t extendedFileType);

#endif