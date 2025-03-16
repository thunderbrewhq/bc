#ifndef BC_OS_FILE_DEFINES_HPP
#define BC_OS_FILE_DEFINES_HPP

#define OS_FILE_ERROR_INVALID_PARAMETER 0x57

#include "bc/file/Error.hpp"

#define OS_FILE_SET_LAST_ERROR(err) ::Blizzard::File::SetLastError(err)

#endif
