#include "bc/os/file/RemoveDirectory.hpp"
#include "bc/file/RemoveDirectory.hpp"
#include "bc/os/file/Defines.hpp"

int32_t OsRemoveDirectory(const char* pathName) {
    if (!pathName) {
        OS_FILE_SET_LAST_ERROR(OS_FILE_ERROR_INVALID_PARAMETER);
        return 0;
    }

    return Blizzard::File::RemoveDirectory(pathName);
}