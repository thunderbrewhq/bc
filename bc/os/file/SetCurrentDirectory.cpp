#include "bc/os/file/SetCurrentDirectory.hpp"
#include "bc/os/file/Defines.hpp"
#include "bc/file/SetWorkingDirectory.hpp"

int32_t OsSetCurrentDirectory(const char* pathName) {
    if (pathName == nullptr) {
        // SetLastError(0x57);
        OS_FILE_SET_LAST_ERROR(OS_FILE_ERROR_INVALID_PARAMETER);
        return 0;
    }

    return Blizzard::File::SetWorkingDirectory(pathName);
}