#ifndef BC_OS_FILE_REMOVE_DIRECTORY_HPP
#define BC_OS_FILE_REMOVE_DIRECTORY_HPP

#include <cstdint>

int32_t OsRemoveDirectoryRecurse(const char* pathName, uint32_t flags);

#endif