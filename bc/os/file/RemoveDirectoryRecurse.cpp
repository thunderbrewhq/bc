#include "bc/os/file/RemoveDirectoryRecurse.hpp"
#include "bc/file/RemoveDirectory.hpp"
#include "bc/os/file/Defines.hpp"

int32_t OsRemoveDirectoryRecurse(const char* pathName, uint32_t flags) {
    if (!pathName) {
        OS_FILE_SET_LAST_ERROR(OS_FILE_ERROR_INVALID_PARAMETER);
        return 0;
    }

    return Blizzard::File::RemoveDirectoryAndContents(pathName, (flags & 0x1) != 0);
}